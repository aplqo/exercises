#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <bit>
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
#include <tuple>
const int maxnm = 2000;
const int maxv = maxnm * 5 + 2, maxe = (maxv + maxnm * 8) * 2;
constexpr int inf = INT_MAX / 2;

struct Edge {
  int from, to, cost;
  mutable int flow;
  const Edge *pre, *rev;

  inline int weight() const;
} ed[maxe];
const Edge *head[maxv + 10], *cur[maxv + 10];
int dis[maxv + 10], h[maxv + 10];
int n, m;

inline void addEdge(const int from, const int to, const int flow,
                    const int cost)
{
  static Edge* cur = ed;
  new (cur) Edge{from, to, cost, flow, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{to, from, -cost, 0, head[to], cur - 1};
  head[to] = cur++;
}
inline int Edge::weight() const { return h[from] + cost - h[to]; }
bool dijkstra(const int s, const int t, const int n)
{
  typedef std::pair<int, int> Pair;
  static bool vis[maxv + 10];
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> q;
  std::memset(vis, 0, sizeof(vis[0]) * n);
  std::fill(dis, dis + n, inf);
  q.emplace(0, s);
  dis[s] = 0;
  while (!q.empty()) {
    const Pair cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const Edge* i = head[cur.second]; i; i = i->pre)
      if (i->flow && !vis[i->to])
        if (const int nxt = cur.first + i->weight(); dis[i->to] > nxt) {
          dis[i->to] = nxt;
          q.emplace(nxt, i->to);
        }
  }
  return vis[t];
}
int augment(const int x, const int cap, const int sink)
{
  static bool vis[maxv + 10];
  if (x == sink || !cap) return cap;
  vis[x] = true;
  int rst = cap;
  for (const Edge*& i = cur[x]; rst && i; i = i->pre)
    if (i->flow && !vis[i->to] && dis[x] + i->weight() == dis[i->to]) {
      const int t = augment(i->to, std::min(rst, i->flow), sink);
      i->flow -= t;
      i->rev->flow += t;
      rst -= t;
    }
  vis[x] = false;
  return cap - rst;
}
std::pair<int, int> ssp(const int source, const int sink, const int n)
{
  int cost = 0, flow = 0;
  while (dijkstra(source, sink, n)) {
    std::memcpy(cur, head, sizeof(const Edge*) * n);
    const int t = augment(source, inf, sink);
    for (int i = 0; i < n; ++i)
      if (dis[i] != inf) h[i] += dis[i];
    cost += t * h[sink];
    flow += t;
  }
  return std::make_pair(flow, cost);
}

enum Offset { Up, Right, Down, Left, Center };
int offset[5];
inline int id(const int x, const int y) { return x * m + y; }
/*!
 * @tparam rev true: y->x false: x->y
 */
template <bool rev>
inline void linkVertex(const int x, const int y, const int flow, const int cost)
{
  if constexpr (rev)
    addEdge(y, x, flow, cost);
  else
    addEdge(x, y, flow, cost);
}
template <bool isSink>
inline void linkEndpoint(const int cid, const int st, const int flow)
{
  linkVertex<isSink>(st, cid, flow, 0);
}
static void linkGrid(const unsigned int n, const unsigned int m)
{
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < m; ++j)
      if (!((i & 0x01) ^ (j & 0x01))) {
        const int cid = id(i, j);
        if (j) addEdge(cid + offset[Left], id(i, j - 1) + offset[Right], 1, 0);
        if (j + 1 < m)
          addEdge(cid + offset[Right], id(i, j + 1) + offset[Left], 1, 0);
        if (i) addEdge(cid + offset[Up], id(i - 1, j) + offset[Down], 1, 0);
        if (i + 1 < n)
          addEdge(cid + offset[Down], id(i + 1, j) + offset[Up], 1, 0);
      }
}
inline void initPlug(const int cid, const unsigned int typ, int plug[5])
{
  for (unsigned int i = 0; i < 4; ++i)
    plug[i] = cid + offset[(i + typ) & 0x3u];
  plug[Center] = cid + offset[Center];
}
template <bool linkSink>
void linkCell(const int cid, const unsigned int typ, const int st)
{
  int vertex[5], rotate = 0;
#define addRotate() \
  ++rotate;         \
  [[fallthrough]]
#define link(t1, t2, cst) linkVertex<linkSink>(vertex[t1], vertex[t2], 1, cst)
#define plug() initPlug(cid, rotate, vertex)

  switch (typ) {
    case 0b1000:
      addRotate();
    case 0b0100:
      addRotate();
    case 0b0010:
      addRotate();
    case 0b0001:
      plug();
      link(Center, Up, 0);
      link(Up, Left, 1);
      link(Up, Right, 1);
      link(Up, Down, 2);
      break;
    case 0b1010:
      addRotate();
    case 0b0101:
      plug();
      link(Center, Up, 0);
      link(Center, Down, 0);
      break;
    case 0b1100:
      addRotate();
    case 0b0110:
      addRotate();
    case 0b0011:
      addRotate();
    case 0b1001:
      plug();
      link(Center, Left, 0);
      link(Center, Up, 0);
      link(Up, Down, 1);
      link(Left, Right, 1);
      break;
    case 0b1110:
      addRotate();
    case 0b0111:
      addRotate();
    case 0b1011:
      addRotate();
    case 0b1101:
      plug();
      link(Center, Up, 0);
      link(Center, Left, 0);
      link(Center, Down, 0);
      link(Up, Right, 1);
      link(Left, Right, 2);
      link(Down, Right, 1);
      break;
    case 0b1111:
      for (int i = 0; i < 4; ++i)
        linkVertex<linkSink>(cid + offset[Center], cid + offset[i], 1, 0);
      break;
    default:
      return;
  }
  linkEndpoint<linkSink>(cid + offset[Center], st, std::popcount(typ));

#undef addRotate
#undef link
#undef plug
}

void printGraph(const int n, const int m)
{
  static const auto print = [](const int i, const int off, const char* str) {
    std::cout << "\t[" << str << "]: " << i + offset[off] << "\n";
  };
  static const auto printEdge = [](const int p) {
    for (const Edge* i = head[p]; i; i = i->pre)
      if (i->flow)
        std::clog << i->from << "->" << i->to << " " << i->cost << "\n";
  };
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j) {
      const int cid = id(i, j);
      std::cout << "(" << i << "," << j << "): \n";
      print(cid, Up, "Up");
      print(cid, Right, "Right");
      print(cid, Down, "Down");
      print(cid, Left, "Left");
      print(cid, Center, "Center");
    }
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j) {
      const int cid = id(i, j);
      for (int t = 0; t < 5; ++t)
        printEdge(cid + offset[t]);
    }
  const int src = n * m * 5;
  printEdge(src);
  printEdge(src + 1);
}

static std::tuple<int, int, int> buildGraph(const int n, const int m)
{
  const int size = n * m, source = n * m * 5, sink = source + 1;
  for (int i = 1; i < 5; ++i)
    offset[i] = offset[i - 1] + size;
  linkGrid(n, m);

  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < m; ++j) {
      const int cid = id(i, j);
      unsigned int typ;
      std::cin >> typ;
      if ((i & 0x01) ^ (j & 0x01))
        linkCell<true>(cid, typ, sink);
      else
        linkCell<false>(cid, typ, source);
    }

  // printGraph(n, m);

  return {source, sink, sink + 1};
}
bool check(const int source, const int sink)
{
  for (const Edge* i = head[sink]; i; i = i->pre)
    if (i->rev->flow) return false;
  for (const Edge* i = head[source]; i; i = i->pre)
    if (i->flow) return false;
  return true;
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  std::cin >> n >> m;
  const auto [s, t, sz] = buildGraph(n, m);
  const auto [f, c] = ssp(s, t, sz);
  if (!check(s, t))
    std::cout << "-1\n";
  else
    std::cout << c << "\n";
  return 0;
}
