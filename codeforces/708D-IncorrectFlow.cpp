#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
const int maxN = 100, maxM = 100;
const int maxV = maxN + 2, maxE = maxM * 3 + maxN + 1;

namespace Network {
typedef long long Flow;
typedef long long Cost;
typedef int EdgeCost;
constexpr Flow flowInf = std::numeric_limits<Flow>::max();
constexpr EdgeCost costInf = std::numeric_limits<EdgeCost>::max();

struct GraphInfo {
  int n, source, sink;
  Cost base;
};
struct Edge {
  int from, to;
  EdgeCost cost;
  mutable Flow flow;
  const Edge *pre, *rev;
} edge[maxE * 2 + 10];
const Edge *head[maxV + 10], *cur[maxV + 10];
EdgeCost h[maxV + 10], dis[maxV + 10];

void addEdge(const int from, const int to, const Flow flow, const EdgeCost cost)
{
  static Edge *cur = edge;
  new (cur) Edge{from, to, cost, flow, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{to, from, -cost, 0, head[to], cur - 1};
  head[to] = cur++;
}

bool dijkstra(const int n, const int source, const int sink)
{
  static bool vis[maxV + 10];
  typedef std::pair<EdgeCost, int> Pair;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> q;
  std::memset(vis, 0, sizeof(vis[0]) * n);
  std::fill(dis, dis + n, costInf);
  dis[source] = 0;
  q.emplace(0, source);
  while (!q.empty()) {
    const Pair cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const Edge *i = head[cur.second]; i; i = i->pre) {
      if (!i->flow) continue;
      const long long w = h[i->from] - h[i->to] + i->cost;
      if (dis[i->to] > cur.first + w) {
        dis[i->to] = cur.first + w;
        q.emplace(dis[i->to], i->to);
      }
    }
  }
  return vis[sink];
}
Flow augment(const int x, const Flow cap, const int sink)
{
  static bool vis[maxV + 10];
  if (x == sink || !cap) return cap;
  vis[x] = true;
  Flow rst = cap;
  for (const Edge *&i = cur[x]; i; i = i->pre) {
    const EdgeCost w = h[i->from] + i->cost - h[i->to];
    if (i->flow && !vis[i->to] && dis[i->to] == dis[x] + w) {
      const Flow d = augment(i->to, std::min(i->flow, rst), sink);
      i->flow -= d;
      i->rev->flow += d;
      rst -= d;
      if (!rst) break;
    }
  }
  vis[x] = false;
  return cap - rst;
}

std::pair<Flow, Cost> sap(const GraphInfo g)
{
  Flow mxf = 0;
  Cost mnc = 0;
  while (dijkstra(g.n, g.source, g.sink)) {
    std::memcpy(cur, head, sizeof(head[0]) * g.n);
    const Flow f = augment(g.source, flowInf, g.sink);
    for (int i = 0; i < g.n; ++i)
      if (dis[i] != costInf) h[i] += dis[i];
    mxf += f;
    mnc += h[g.sink] * f;
  }
  return std::make_pair(mxf, mnc + g.base);
}
}  // namespace Network
namespace GraphBuilder {
struct Edge {
  int from, to, flow, capacity;
} edge[maxM + 10];
Network::Flow exceed[maxN + 10];

void addEdge(const int from, const int to, const int flow, const int capacity)
{
  static Edge *endPtr = edge;
  new (endPtr++) Edge{from, to, flow, capacity};
  exceed[from] -= flow;
  exceed[to] += flow;
}
Network::GraphInfo build(const int n, const int m)
{
  using Network::addEdge, Network::flowInf;
  const int source = n, sink = source + 1;
  for (int i = 0; i < n; ++i)
    if (exceed[i] > 0)
      addEdge(source, i, exceed[i], 0);
    else if (exceed[i] < 0)
      addEdge(i, sink, -exceed[i], 0);
  addEdge(n - 1, 0, flowInf, 0);
  Network::Cost base = 0;
  for (const Edge *i = edge; i < edge + m; ++i) {
    addEdge(i->from, i->to, flowInf, 2);
    if (i->flow > i->capacity) {
      base += i->flow - i->capacity;
      addEdge(i->to, i->from, i->capacity, 1);
      addEdge(i->to, i->from, i->flow - i->capacity, 0);
    }
    else {
      addEdge(i->to, i->from, i->flow, 1);
      addEdge(i->from, i->to, i->capacity - i->flow, 1);
    }
  }
  return Network::GraphInfo{sink + 1, source, sink, base};
}
}  // namespace GraphBuilder

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int u, v, f, c;
    std::cin >> u >> v >> c >> f;
    GraphBuilder::addEdge(u - 1, v - 1, f, c);
  }
  std::cout << Network::sap(GraphBuilder::build(n, m)).second << "\n";
  return 0;
}