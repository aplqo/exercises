#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
const int maxn = 1e5, maxm = 1e5, maxCell = 1e5;
const int maxv = maxCell + 4, maxe = maxCell * 4 + 4 * maxCell;
const int inf = std::numeric_limits<int>::max() / 2;

namespace Network {
struct GraphInfo {
  int vertex, source, sink;
};
struct Edge {
  int from, to;
  mutable int flow;
  const Edge *pre, *rev;
} ed[maxe * 2 + 10], *end = ed;
const Edge *head[maxv + 10], *cur[maxv + 10];
int exceed[maxv + 10], dep[maxv + 10];

enum class EdgeType { Normal, Required };
inline void addGraphEdge(const int from, const int to, const int flow)
{
  new (end) Edge{from, to, flow, head[from], end + 1};
  head[from] = end++;
  new (end) Edge{to, from, 0, head[to], end - 1};
  head[to] = end++;
}
template <EdgeType et>
inline void addUnitEdge(const int from, const int to)
{
  if constexpr (et == EdgeType::Required) {
    --exceed[from];
    ++exceed[to];
  }
  else
    addGraphEdge(from, to, 1);
}

bool bfs(const int n, const int source, const int sink)
{
  std::queue<int> q;
  std::fill(dep, dep + n, inf);
  q.push(source);
  dep[source] = 0;
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
  if (x == sink || !cap) return cap;
  int rst = cap;
  for (; cur[x]; cur[x] = cur[x]->pre) {
    const Edge *i = cur[x];
    if (i->flow && dep[i->to] == dep[x] + 1) {
      const int d = dfs(i->to, std::min(rst, i->flow), sink);
      i->flow -= d;
      i->rev->flow += d;
      rst -= d;
      if (!rst) break;
    }
  }
  return cap - rst;
}

int dinic(const GraphInfo gi)
{
  int ret = 0;
  while (bfs(gi.vertex, gi.source, gi.sink)) {
    std::memcpy(cur, head, sizeof(cur[0]) * gi.vertex);
    ret += dfs(gi.source, inf, gi.sink);
  }
  return ret;
}
bool hasSolution(const int virtSource)
{
  for (const Edge *i = head[virtSource]; i; i = i->pre)
    if (i->flow) return false;
  return true;
}
void init(const int n)
{
  end = ed;
  std::memset(head, 0, sizeof(head[0]) * (n + 2));
  std::memset(exceed, 0, sizeof(exceed[0]) * (n + 2));
}
int findMatch(const int v, const int limit)
{
  for (const Edge *i = head[v]; i; i = i->pre)
    if (!i->flow && i->to < limit) return i->to;
  return limit;
}

bool boundMaxFlow(const GraphInfo gi)
{
  const int virtSource = gi.vertex, virtSink = virtSource + 1;
  addGraphEdge(gi.sink, gi.source, inf);
  for (int i = 0; i < gi.vertex; ++i)
    if (exceed[i] > 0)
      addGraphEdge(virtSource, i, exceed[i]);
    else if (exceed[i] < 0)
      addGraphEdge(i, virtSink, -exceed[i]);
  dinic(GraphInfo{virtSink + 1, virtSource, virtSink});
  return hasSolution(virtSource);
}
}  // namespace Network

int n, m;
enum Direction { Left, Right, Down, Up, None };
const Direction reverse[]{Right, Left, Up, Down, None};
struct Step {
  int dx, dy;

  inline bool operator==(const Step s) const
  {
    return dx == s.dx && dy == s.dy;
  }
};
const Step step[]{{0, -1}, {0, 1}, {1, 0}, {-1, 0}};
const char directionName[] = {'L', 'R', 'D', 'U'};
struct Position {
  int x, y;

  inline static Position fromId(const int id)
  {
    const div_t d = div(id, m);
    return Position{d.quot, d.rem};
  }
  inline Position operator+(const struct Step s) const
  {
    return Position{x + s.dx, y + s.dy};
  }
  inline Step operator-(const Position r) const
  {
    return Step{x - r.x, y - r.y};
  }
  inline bool isValid() const { return x >= 0 && y >= 0 && x < n && y < m; }
  inline int id() const { return x * m + y; }
};
struct Cell {
  Direction next;
  int sum, val;
} cell[maxCell + 10];

namespace GraphBuilder {
template <bool rev, Network::EdgeType type>
inline void linkVertex(const int from, const int to)
{
  if constexpr (rev)
    Network::addUnitEdge<type>(to, from);
  else
    Network::addUnitEdge<type>(from, to);
}
template <bool toSink>
inline void linkCell(const Position pos, const int endpoint)
{
  using Network::EdgeType;
  const int cid = pos.id();
  Cell &cur = cell[cid];
  cur.next = Direction::None;
  for (int k = 0; k < 4; ++k)
    if (const auto nxt = pos + step[k]; nxt.isValid()) {
      const int nextId = nxt.id();
      if (cell[nextId].sum < cur.sum)
        cur.next = static_cast<Direction>(k);
      else if (!toSink && cell[nextId].sum == cur.sum)
        Network::addUnitEdge<EdgeType::Normal>(cid, nextId);
    }
  if (cur.next == Direction::None)
    linkVertex<toSink, EdgeType::Required>(endpoint, cid);
  else
    linkVertex<toSink, EdgeType::Normal>(endpoint, cid);
}
inline bool isLinkSink(const Position &p)
{
  return (p.x & 0x01) ^ (p.y & 0x01);
}
static Network::GraphInfo build(const int n, const int m)
{
  const int source = n * m, sink = source + 1;
  Network::init(sink + 1);
  for (int i = 0; i < n; ++i)
    for (Position pos{i, 0}; pos.y < m; ++pos.y)
      if (isLinkSink(pos))
        linkCell<true>(pos, sink);
      else
        linkCell<false>(pos, source);
  return Network::GraphInfo{sink + 1, source, sink};
}
}  // namespace GraphBuilder
void construct(const int n, const int m)
{
  const int lim = n * m;
  for (int i = 0, cid = 0; i < n; ++i)
    for (Position pos{i, 0}; pos.y < m; ++pos.y, ++cid)
      if (Cell &cur = cell[cid]; cur.next != Direction::None)
        cur.val = cur.sum - cell[(pos + step[cur.next]).id()].sum;

  for (int i = 0, cid = 0; i < n; ++i)
    for (Position pos{i, 0}; pos.y < m; ++pos.y, ++cid)
      if (!GraphBuilder::isLinkSink(pos))
        if (const int nxt = Network::findMatch(cid, lim); nxt < lim) {
          Cell &cur = cell[cid];
          const int typ =
              std::find(step, step + 4, Position::fromId(nxt) - pos) - step;
          cur.val = cur.sum - 1;
          cur.next = static_cast<Direction>(typ);
          cell[nxt].val = 1;
          cell[nxt].next = reverse[typ];
        }
}
template <class Func>
void printMatrix(const int n, const int m, Func fun)
{
  const Cell *ptr = cell;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j, ++ptr)
      std::cout << fun(*ptr) << ' ';
    std::cout.put('\n');
  }
}

void solve()
{
  std::cin >> n >> m;
  for (Cell *i = cell; i < cell + n * m; ++i)
    std::cin >> i->sum;
  if (!Network::boundMaxFlow(GraphBuilder::build(n, m)))
    std::cout << "NO\n";
  else {
    construct(n, m);
    std::cout << "YES\n";
    printMatrix(n, m, [](const Cell &i) { return i.val; });
    printMatrix(n, m, [](const Cell &i) { return directionName[i.next]; });
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = t; i; --i)
    solve();
  return 0;
}