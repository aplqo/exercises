#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
const int maxn = 10000, maxq = 10000, maxl = 14;
const int maxv = maxn * 4 + maxl * maxq * 2 * 2 + maxn + 2, maxe = maxv * 2;
const int inf = INT_MAX;

namespace Network {
struct GraphStat {
  int n, source, sink;
};
struct Edge {
  int from, to;
  mutable int flow;
  union {
    const Edge *ptr;
    int id;
  } rev;
} ed[maxe * 2 + 10];
std::vector<Edge> next[maxv + 10];
int cur[maxv + 10];
int dep[maxv + 10];

inline void addEdge(const int from, const int to, const int flow)
{
  next[from].push_back(
      Edge{from, to, flow, {.id = static_cast<int>(next[to].size())}});
  next[to].push_back(
      Edge{to, from, 0, {.id = static_cast<int>(next[from].size() - 1)}});
}

bool bfs(const int source, const int sink)
{
  static int vis[maxv + 10], sta;
  std::queue<int> q;
  ++sta;
  vis[sink] = sta;
  dep[sink] = 0;
  q.push(sink);
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    for (auto &j : next[cur])
      if (j.rev.ptr->flow && vis[j.to] != sta) {
        dep[j.to] = dep[cur] + 1;
        vis[j.to] = sta;
        q.push(j.to);
      }
  }
  return vis[source] == sta;
}
int dfs(const int x, const int cap, const int sink)
{
  if (!cap || x == sink) return cap;
  int rst = cap;
  const int lim = next[x].size();
  for (int &i = cur[x]; i < lim && rst; ++i) {
    const Edge &c = next[x][i];
    if (dep[c.to] + 1 == dep[x] && c.flow) {
      const int delt = dfs(c.to, std::min(rst, c.flow), sink);
      c.flow -= delt;
      c.rev.ptr->flow += delt;
      rst -= delt;
      if (!rst) break;
    }
  }
  return cap - rst;
}
static void init(const int n)
{
  for (int i = 0; i < n; ++i)
    for (auto &j : next[i])
      j.rev.ptr = &next[j.to][j.rev.id];
}
int maxFlow(const GraphStat stat)
{
  int ret = 0;
  init(stat.n);
  while (bfs(stat.source, stat.sink)) {
    std::memset(cur, 0, sizeof(cur[0]) * stat.n);
    ret += dfs(stat.source, inf, stat.sink);
  }
  return ret;
}
}  // namespace Network
namespace GraphBuilder {
struct Side {
  int left, right;
};
struct {
  std::vector<Side> add, remove;
} modify[maxn + 10];
int virtVertex, n;
class SegmentTree {
 public:
  void build(const int l, const int r, SegmentTree *&cur);
  template <bool add>
  void modify(const int l, const int r);

  int current;

 private:
  int lv, mv, rv;
  int full;
  SegmentTree *lc, *rc;
} nodes[maxn * 4 + 10], *root = nodes;

void SegmentTree::build(const int l, const int r, SegmentTree *&cur)
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  if (r - l == 1) {
    full = current = l + n;
    return;
  }
  full = current = virtVertex++;
  lc = cur++;
  lc->build(l, mv, cur);
  Network::addEdge(full, lc->full, inf);
  rc = cur++;
  rc->build(mv, r, cur);
  Network::addEdge(full, rc->full, inf);
}
template <bool add>
void SegmentTree::modify(const int l, const int r)
{
  if (l == lv && r == rv) {
    current = add ? full : 0;
    return;
  }
  if (r <= mv)
    lc->modify<add>(l, r);
  else if (l >= mv)
    rc->modify<add>(l, r);
  else {
    lc->modify<add>(l, mv);
    rc->modify<add>(mv, r);
  }
  if (lc->current || rc->current) {
    using Network::addEdge;
    current = virtVertex++;
    if (lc->current) addEdge(current, lc->current, inf);
    if (rc->current) addEdge(current, rc->current, inf);
  }
  else
    current = 0;
}

static void init(const int n)
{
  GraphBuilder::n = n;
  virtVertex = n * 2;
}
void addRectangle(const int x1, const int y1, const int x2, const int y2)
{
  modify[x1 - 1].remove.push_back(Side{y1 - 1, y2});
  modify[x2].add.push_back(Side{y1 - 1, y2});
}
Network::GraphStat build(const int n)
{
  using Network::addEdge;
  {
    SegmentTree *cur = nodes + 1;
    root->build(0, n, cur);
  }
  for (int i = 0; i < n; ++i) {
    for (const auto &j : modify[i].add)
      root->modify<true>(j.left, j.right);
    for (const auto &j : modify[i].remove)
      root->modify<false>(j.left, j.right);
    if (root->current) addEdge(i, root->current, inf);
  }
  const int source = virtVertex++, sink = virtVertex++;
  for (int i = 0; i < n; ++i) {
    addEdge(source, i, 1);
    addEdge(i + n, sink, 1);
  }
  return Network::GraphStat{virtVertex, source, sink};
}
}  // namespace GraphBuilder

int main()
{
  std::ios::sync_with_stdio(false);
  int n, q;
  std::cin >> n >> q;
  GraphBuilder::init(n);
  for (int i = 0; i < q; ++i) {
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    GraphBuilder::addRectangle(x1, y1, x2, y2);
  }
  std::cout << Network::maxFlow(GraphBuilder::build(n)) << "\n";
  return 0;
}