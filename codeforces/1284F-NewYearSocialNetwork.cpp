#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
#include <vector>
const int maxN = 250000, maxL = 18;

class Tree {
 public:
  struct Edge {
    int to;
    const Edge *pre;
  };
  const Edge *head[maxN + 10];
  void addEdge(const int from, const int to)
  {
    new (cur) Edge{to, head[from]};
    head[from] = cur++;
    new (cur) Edge{from, head[to]};
    head[to] = cur++;
  }

 private:
  Edge ed[maxN * 2 + 10], *cur = ed;
} t1, t2;
struct Edge {
  int u, v;
  friend std::ostream &operator<<(std::ostream &os, const Edge e)
  {
    return os << e.u << " " << e.v;
  }
};
struct Answer {
  Edge tree1, tree2;
} answer[maxN + 1], *cur = answer;
int depth[maxN + 10];

namespace Disjoint {
int fa[maxN + 10], size[maxN + 10], top[maxN + 10];

static void init(const int n)
{
  std::iota(fa, fa + n + 1, 0);
  std::fill(size, size + n + 1, 1);
  std::memcpy(top, fa, sizeof(fa[0]) * (n + 1));
}
int find(const int x) { return fa[x] == x ? x : fa[x] = find(fa[x]); }
inline bool isConnect(const int x, const int y) { return find(x) == find(y); }
void merge(int x, int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (size[x] < size[y]) std::swap(x, y);
  fa[y] = x;
  size[x] += size[y];
  top[x] = depth[top[x]] < depth[top[y]] ? top[x] : top[y];
}
}  // namespace Disjoint
namespace Lift {
int fa[maxN + 10][maxL + 1], lg[maxN + 10];

static void dfs(const int x, const int f)
{
  depth[x] = depth[f] + 1;
  fa[x][0] = f;
  for (int i = 1; i <= lg[depth[x]] + 1; ++i)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  for (auto i = t1.head[x]; i; i = i->pre)
    if (i->to != f) dfs(i->to, x);
}
static void initLog(const int n)
{
  lg[0] = -1;
  for (int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
static void init(const int n)
{
  initLog(n);
  dfs(1, 0);
}
int lca(int a, int b)
{
  if (depth[a] != depth[b]) {
    if (depth[a] > depth[b]) std::swap(a, b);
    int d = depth[b] - depth[a];
    for (int i = 0; d; d >>= 1, ++i)
      if (d & 0x01) b = fa[b][i];
  }
  if (a == b) return a;
  for (int i = lg[depth[a]] + 1; i;) {
    const int m = i - 1;
    if (fa[a][m] == fa[b][m])
      --i;
    else {
      a = fa[a][m];
      b = fa[b][m];
    }
  }
  return fa[a][0];
}
int binaryLift(int x, const int up)
{
  for (int i = maxL; i >= 0; --i) {
    const int p = fa[x][i];
    if (depth[p] > depth[up] && !Disjoint::isConnect(p, up)) x = p;
  }
  return x;
}
}  // namespace Lift

int fa[maxN + 10];
std::vector<int> layer[maxN + 10];
void addReplace(const Edge et1, const Edge et2)
{
  Disjoint::merge(et1.u, et1.v);
  new (cur++) Answer{et1, et2};
}
void findReplace(const int leaf, const int fa)
{
  const int lca = Lift::lca(leaf, fa);
  const int top = Disjoint::top[Disjoint::find(leaf)];
  if (depth[top] > depth[lca])
    addReplace(Edge{top, Lift::fa[top][0]}, Edge{leaf, fa});
  else {
    const int edge = Lift::binaryLift(fa, lca);
    addReplace(Edge{edge, Lift::fa[edge][0]}, Edge{leaf, fa});
  }
}
void dfs(const int x, const int f, const int depth)
{
  layer[depth].push_back(x);
  fa[x] = f;
  for (auto i = t2.head[x]; i; i = i->pre)
    if (i->to != f) dfs(i->to, x, depth + 1);
}
static void init(const int n)
{
  Lift::init(n);
  Disjoint::init(n);
}
void solve(const int n)
{
  dfs(1, 0, 1);
  for (int i = n; i > 1; --i)
    for (int j : layer[i])
      findReplace(j, fa[j]);
}
void readTree(Tree &t, const int n)
{
  for (int i = 1; i < n; ++i) {
    int u, v;
    std::cin >> u >> v;
    t.addEdge(u, v);
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  readTree(t1, n);
  readTree(t2, n);
  init(n);
  solve(n);
  std::cout << n - 1 << "\n";
  for (const Answer *i = answer; i < cur; ++i)
    std::cout << i->tree1 << " " << i->tree2 << "\n";
  return 0;
}