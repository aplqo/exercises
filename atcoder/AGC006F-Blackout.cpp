#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
unsigned int color[maxn + 10], degree[maxn + 10];
unsigned int countColor[maxn + 10][3], countEdge[maxn + 10];
bool vis[maxn + 10], fail[maxn + 10];

namespace Disjoint {
unsigned int fa[maxn + 10], size[maxn + 10];

static void init(const unsigned int n)
{
  fill(size, size + n + 1, 1);
  iota(fa, fa + n + 1, 0);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
inline void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (size[x] < size[y]) swap(x, y);
  fa[y] = x;
  size[x] += size[y];
}
}  // namespace Disjoint
inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  static Edge* cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  ++degree[to];
}
bool dfs(const unsigned int x)
{
  vis[x] = true;
  const unsigned int rt = Disjoint::find(x);
  ++countColor[rt][color[x]];
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to, ncol = (color[x] + i->val) % 3;
    ++countEdge[rt];
    if (vis[to]) {
      if (color[to] != ncol) return false;
    }
    else {
      color[to] = ncol;
      if (!dfs(to)) return false;
    }
  }
  return true;
}
unsigned long long count(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i) {
    const unsigned int rt = Disjoint::find(i);
    if (!vis[i] && !fail[rt]) fail[rt] = !dfs(i);
  }
  unsigned long long ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    if (Disjoint::find(i) == i) {
      if (fail[i])
        ret += static_cast<unsigned long long>(Disjoint::size[i]) *
               Disjoint::size[i];
      else if (*min_element(countColor[i], countColor[i] + 3))
        ret += static_cast<unsigned long long>(countColor[i][0]) *
                   countColor[i][1] +
               static_cast<unsigned long long>(countColor[i][1]) *
                   countColor[i][2] +
               static_cast<unsigned long long>(countColor[i][2]) *
                   countColor[i][0];
      else
        ret += countEdge[i] >> 1;
    }
  return ret;
}

int main()
{
#ifndef APTEST
  ios::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  Disjoint::init(n);
  for (unsigned int i = m; i; --i) {
    unsigned int a, b;
    cin >> a >> b;
    addEdge(a, b, 1);
    addEdge(b, a, 2);
    Disjoint::merge(a, b);
  }
  cout << count(n) << "\n";
  return 0;
}