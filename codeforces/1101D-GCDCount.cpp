#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
const int maxN = 2e5, maxV = 2e5;

struct OrigEdge {
  int u, v;
} ed[maxN + 10];
std::vector<const OrigEdge *> edge[maxV + 10];
std::vector<int> factor[maxV + 10];
int a[maxN + 10];

namespace Graph {
struct Edge {
  int to;
  const Edge *pre;
} ed[maxN * 2 + 10], *cur = ed;
const Edge *head[maxN + 10];
struct {
  int id, included;
} vertex[maxN + 10];
int vis[maxN + 10];
int graphId, cnt;

inline int getId(const int x)
{
  if (vertex[x].included != graphId) {
    vertex[x].included = graphId;
    vertex[x].id = cnt;
    head[cnt++] = nullptr;
  }
  return vertex[x].id;
}
inline void addEdge(int from, int to)
{
  from = getId(from);
  to = getId(to);
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
inline void init()
{
  cur = ed;
  ++graphId;
  cnt = 0;
}

struct DPResult {
  int distance, chain;
};
DPResult dfs(const int x, const int f)
{
  vis[x] = graphId;
  DPResult ret{1, 1};
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    const DPResult r = dfs(to, x);
    ret.distance = std::max({ret.distance, ret.chain + r.chain, r.distance});
    ret.chain = std::max(ret.chain, r.chain + 1);
  }
  return ret;
}
int dp(const std::vector<const OrigEdge *> &edges)
{
  init();
  for (const auto i : edges)
    addEdge(i->u, i->v);
  int ret = 0;
  for (int i = 0; i < cnt; ++i)
    if (vis[i] != graphId) ret = std::max(ret, dfs(i, 0).distance);
  return ret;
}
}  // namespace Graph
static void initFactor(const int v)
{
  for (int i = 2; i <= v; ++i)
    if (factor[i].empty())
      for (int j = i; j <= v; j += i)
        factor[j].push_back(i);
}
inline void addEdge(const int u, const int v)
{
  static OrigEdge *cur = ed;
  new (cur) OrigEdge{u, v};
  for (const int p : factor[std::gcd(a[u], a[v])])
    edge[p].push_back(cur);
  ++cur;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, max = 0;
  std::cin >> n;
  for (int *i = a + 1; i < a + 1 + n; ++i) {
    std::cin >> *i;
    max = std::max(*i, max);
  }
  if (max == 1) {
    std::cout << "0\n";
    return 0;
  }
  initFactor(max);
  for (int i = 1; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    addEdge(x, y);
  }
  int ans = 1;
  for (int i = 2; i <= max; ++i)
    if (!edge[i].empty()) ans = std::max(ans, Graph::dp(edge[i]));
  std::cout << ans << "\n";
  return 0;
}
