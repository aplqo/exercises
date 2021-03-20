#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
constexpr int maxn = 100000, maxm = 100000, maxPos = maxn + maxm;
constexpr int maxv = maxPos * 2 + 4, maxe = maxPos + maxPos * 2 + maxv + 1;
const int inf = INT_MAX / 2;

[[noreturn]] void fail()
{
  std::cout << "-1\n";
  std::exit(0);
}
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
int exceed[maxv + 10], dep[maxv + 10];

inline const Edge* addGraphEdge(const int from, const int to, const int flow)
{
  static Edge* cur = ed;
  new (cur) Edge{to, from, 0, head[to], cur + 1};
  head[to] = cur++;
  new (cur) Edge{from, to, flow, head[from], cur - 1};
  head[from] = cur;
  return cur++;
}
inline void addEdge(const int from, const int to, const int low, const int high)
{
  if (high < low) fail();
  exceed[from] -= low;
  exceed[to] += low;
  addGraphEdge(from, to, high - low);
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
    for (const Edge* i = head[cur]; i; i = i->pre)
      if (i->flow && dep[i->to] == inf) {
        dep[i->to] = dep[cur] + 1;
        q.push(i->to);
      }
  }
  return dep[sink] != inf;
}
int dfs(const int x, const int cap, const int sink)
{
  if (x == sink || !cap) return cap;
  int rst = cap;
  for (const Edge*& i = cur[x]; i; i = i->pre)
    if (i->flow && dep[i->to] == dep[x] + 1) {
      const int d = dfs(i->to, std::min(i->flow, rst), sink);
      i->flow -= d;
      i->rev->flow += d;
      rst -= d;
      if (!rst) break;
    }
  return cap - rst;
}
int dinic(const GraphInfo gi)
{
  int ret = 0;
  while (bfs(gi.vertex, gi.source, gi.sink)) {
    std::memcpy(cur, head, sizeof(head[0]) * gi.vertex);
    ret += dfs(gi.source, inf, gi.sink);
  }
  return ret;
}
bool check(const int virtSource)
{
  for (const Edge* i = head[virtSource]; i; i = i->pre)
    if (i->flow) return false;
  return true;
}

std::pair<int, bool> boundMaxFlow(const GraphInfo gi)
{
  const int virtSource = gi.vertex, virtSink = virtSource + 1;
  for (int i = 0; i < gi.vertex; ++i)
    if (exceed[i] > 0)
      addGraphEdge(virtSource, i, exceed[i]);
    else if (exceed[i] < 0)
      addGraphEdge(i, virtSink, -exceed[i]);
  addGraphEdge(gi.sink, gi.source, inf);

  dinic(GraphInfo{virtSink + 1, virtSource, virtSink});
  if (!check(virtSource))
    return std::make_pair(0, false);
  else
    return std::make_pair(dinic(gi), true);
}
}  // namespace Network
enum { X = 0, Y = 1 };
struct Constraint {
  int val, d;

  inline friend std::istream& operator>>(std::istream& is, Constraint& c)
  {
    return is >> c.val >> c.d;
  }
} con[2][maxm + 10], *end[2]{con[0], con[1]};
struct Point {
  int position[2];
  bool select;
  const Network::Edge* edge;
} shield[maxn + 10];
int n;

namespace GraphBuilder {

template <int typ>
int discretize(const int n)
{
  static int buf[maxn * 2 + 10];
  int* ptr = buf;

  for (const Constraint* i = con[typ]; i < end[typ]; ++i)
    *(ptr++) = i->val;
  for (const Point* i = shield; i < shield + n; ++i)
    *(ptr++) = i->position[typ];
  std::sort(buf, ptr);
  ptr = std::unique(buf, ptr);

  const auto get = [ptr](const int v) -> int {
    return std::lower_bound(buf, ptr, v) - buf;
  };
  for (Constraint* i = con[typ]; i < end[typ]; ++i)
    i->val = get(i->val);
  for (Point* i = shield; i < shield + n; ++i)
    i->position[typ] = get(i->position[typ]);
  return ptr - buf;
}

template <bool rev>
void linkVertex(int from, int to, const int limit, const int count)
{
  using Network::addEdge;
  if constexpr (rev) std::swap(from, to);
  if (limit == inf)
    Network::addGraphEdge(from, to, inf);
  else
    addEdge(from, to, std::max(0, (count - limit + 1) / 2),
            (count + limit) / 2);
}
template <bool toSource, int typ>
void linkEndpoint(const int base, const int npos, const int endpoint)
{
  static int limit[maxn * 2 + 10], count[maxn * 2 + 10];
  std::fill(limit, limit + npos, inf);
  for (const Constraint* i = con[typ]; i < end[typ]; ++i)
    limit[i->val] = std::min(limit[i->val], i->d);
  for (const Point* i = shield; i < shield + n; ++i)
    ++count[i->position[typ]];

  for (int i = 0; i < npos; ++i)
    linkVertex<toSource>(base + i, endpoint, limit[i], count[i]);
}
Network::GraphInfo build(const int n)
{
  const int nx = discretize<X>(n), ny = discretize<Y>(n);
  const int source = nx + ny, sink = source + 1;
  for (Point* i = shield; i < shield + n; ++i)
    i->edge = Network::addGraphEdge(i->position[X], i->position[Y] + nx, 1);
  linkEndpoint<true, X>(0, nx, source);
  linkEndpoint<false, Y>(nx, ny, sink);
  return Network::GraphInfo{sink + 1, source, sink};
}
}  // namespace GraphBuilder
template <char cheap, char expensive>
void printSolution(const int n)
{
  for (const Point* i = shield; i < shield + n; ++i)
    std::cout.put(i->edge->flow ? expensive : cheap);
  std::cout.put('\n');
}

int main()
{
  std::ios::sync_with_stdio(false);
  int m;
  long long r, b;
  std::cin >> n >> m >> r >> b;
  for (Point* i = shield; i < shield + n; ++i)
    std::cin >> i->position[X] >> i->position[Y];
  for (int i = 0; i < m; ++i) {
    int type;
    std::cin >> type;
    if (type == 1)
      std::cin >> *(end[X]++);
    else
      std::cin >> *(end[Y]++);
  }
  const auto [val, suc] = Network::boundMaxFlow(GraphBuilder::build(n));
  if (!suc)
    std::cout << "-1\n";
  else {
    if (r < b) {
      std::cout << r * val + b * (n - val) << "\n";
      printSolution<'r', 'b'>(n);
    }
    else {
      std::cout << r * (n - val) + b * val << "\n";
      printSolution<'b', 'r'>(n);
    }
  }
  return 0;
}