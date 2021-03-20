#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
const int maxn = 2e4, maxm = 1e4, maxl = 15;
const int maxv = maxn * maxl + 2 + maxm * maxl, maxe = maxv * 2;
const int inf = INT_MAX / 2;

namespace Network {
struct GraphInfo {
  int vertex, source, sink;
};
struct Edge {
  int from, to;
  mutable int flow;
  const Edge *pre, *rev;
} ed[maxe * 2 + 10];
const Edge *head[maxv + 10], *cur[maxv + 10];
int dep[maxv + 10];

inline void addEdge(const int from, const int to, const int flow)
{
  static Edge *cur = ed;
  new (cur) Edge{from, to, flow, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{to, from, 0, head[to], cur - 1};
  head[to] = cur++;
}

bool bfs(const int n, const int source, const int sink)
{
  std::queue<int> q;
  std::fill(dep, dep + n, inf);
  dep[source] = 0;
  q.push(source);
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    for (const Edge *i = head[cur]; i; i = i->pre)
      if (i->flow && dep[i->to] == inf) {
        dep[i->to] = dep[cur] + 1;
        q.push(i->to);
      }
  }
  return dep[sink] != inf;
}
int dfs(const int x, const int cap, const int sink)
{
  if (!cap || x == sink) return cap;
  int rst = cap;
  for (const Edge *&i = cur[x]; i; i = i->pre)
    if (i->flow && dep[i->to] == dep[x] + 1) {
      const int d = dfs(i->to, std::min(rst, i->flow), sink);
      i->flow -= d;
      i->rev->flow += d;
      rst -= d;
      if (!rst) break;
    }
  return cap - rst;
}

int minCut(const GraphInfo gi)
{
  int ret = 0;
  while (bfs(gi.vertex, gi.source, gi.sink)) {
    std::memcpy(cur, head, sizeof(cur[0]) * gi.vertex);
    ret += dfs(gi.source, inf, gi.sink);
  }
  return ret;
}
void findConnect(const int x, const int lim, bool dest[])
{
  static bool vis[maxv + 10];
  vis[x] = true;
  if (x < lim) dest[x] = true;
  for (const Edge *i = head[x]; i; i = i->pre)
    if (i->flow && !vis[i->to]) findConnect(i->to, lim, dest);
}
}  // namespace Network
namespace Tree {
struct Edge {
  int id, to;
  const Edge *pre;
} ed[maxn * 2 + 10];
const Edge *head[maxn + 10];

inline void addEdge(const int id, const int from, const int to)
{
  static Edge *cur = ed;
  new (cur) Edge{id, to, head[from]};
  head[from] = cur++;
  new (cur) Edge{id, from, head[to]};
  head[to] = cur++;
}
}  // namespace Tree
namespace GraphBuilder {
int lg[maxn + 10];
int virtVertex, n, source;
int vertex[maxn + 10][maxl + 1], fa[maxn + 10][maxl + 1], dep[maxn + 10];

template <class T>
inline void addAuxEdge(const int from, const T to)
{
  Network::addEdge(from, to, inf);
}
template <class T, class... Args>
inline void addAuxEdge(const int from, const T to1, const Args... args)
{
  addAuxEdge(from, to1);
  addAuxEdge(from, args...);
}
static void initLog(const int n)
{
  for (int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void dfs(const int x, const int f)
{
  fa[x][0] = f;
  for (int i = 1; i <= lg[dep[x]]; ++i) {
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
    vertex[x][i] = virtVertex++;
    addAuxEdge(vertex[x][i], vertex[x][i - 1], vertex[fa[x][i - 1]][i - 1]);
  }
  for (const Tree::Edge *i = Tree::head[x]; i; i = i->pre)
    if (i->to != f) {
      dep[i->to] = dep[x] + 1;
      vertex[i->to][0] = i->id;
      dfs(i->to, x);
    }
}
static void linkGuardian(const int n, const int sink)
{
  for (int i = 0; i < n; ++i)
    Network::addEdge(i, sink, 1);
}
static Network::GraphInfo init(const int n, const int m)
{
  virtVertex = n + m - 1;
  GraphBuilder::n = n;
  source = virtVertex++;
  const int sink = virtVertex++;
  initLog(n);
  dfs(0, 0);
  linkGuardian(n - 1, sink);
  return Network::GraphInfo{
      .vertex = virtVertex, .source = source, .sink = sink};
}
void addPath(const int id, int u, int v)
{
  const int vert = id + n - 1;
  Network::addEdge(source, vert, 1);
  if (dep[u] != dep[v]) {
    if (dep[u] > dep[v]) std::swap(u, v);
    int d = dep[v] - dep[u];
    for (int k = 0; d; ++k, d >>= 1)
      if (d & 0x01) {
        addAuxEdge(vert, vertex[v][k]);
        v = fa[v][k];
      }
  }
  if (u == v) return;
  for (int i = lg[dep[u]] + 1; i;) {
    const int mid = i - 1;
    if (fa[u][mid] == fa[v][mid])
      i = mid;
    else {
      addAuxEdge(vert, vertex[u][mid], vertex[v][mid]);
      u = fa[u][mid];
      v = fa[v][mid];
    }
  }
  addAuxEdge(vert, vertex[u][0], vertex[v][0]);
}
}  // namespace GraphBuilder

template <bool v>
void printIf(const bool val[], const int n)
{
  std::cout << std::count(val, val + n, v) << " ";
  for (int i = 0; i < n; ++i)
    if (val[i] == v) std::cout << i + 1 << " ";
  std::cout.put('\n');
}
void printSolution(const int source, const int n, const int m)
{
  static bool vis[maxn + maxm];
  Network::findConnect(source, n + m - 1, vis);
  printIf<false>(vis + n - 1, m);
  printIf<true>(vis, n - 1);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  for (int i = 1; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    Tree::addEdge(i - 1, x - 1, y - 1);
  }
  const auto gi = GraphBuilder::init(n, m);
  for (int i = 0; i < m; ++i) {
    int x, y;
    std::cin >> x >> y;
    GraphBuilder::addPath(i, x - 1, y - 1);
  }
  const int c = Network::minCut(gi);
  std::cout << c << "\n";
  printSolution(gi.source, n, m);
  return 0;
}