#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
#include <tuple>
#include <unordered_map>
const unsigned int maxr = 1e5, maxc = 1e5, maxn = 1e5;
constexpr unsigned int maxv = maxn * 2 + 10, maxe = maxv * 10;
const unsigned int inf = UINT_MAX;

struct Cube {
  unsigned int r, c, w;
} cub[maxn + 10];
struct Edge {
  unsigned int from, to;
  mutable unsigned int flow;
  const Edge *pre, *rev;
} ed[maxe + 10];
const Edge *head[maxv + 10], *from[maxv + 10];
unsigned int dep[maxv + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int flow)
{
  static Edge* cur = ed;
  new (cur) Edge{from, to, flow, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{to, from, 0, head[to], cur - 1};
  head[to] = cur++;
}

void bfs(const unsigned int t)
{
  static bool vis[maxv + 10];
  std::queue<unsigned int> q;
  vis[t] = true;
  dep[t] = 0;
  q.push(t);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (const Edge* i = head[cur]; i; i = i->pre)
      if (!vis[i->from] && i->rev->flow) {
        vis[i->from] = true;
        dep[i->from] = dep[cur] + 1;
        q.push(i->from);
      }
  }
}
unsigned int augment(const unsigned int s, const unsigned int t)
{
  unsigned int ret = UINT_MAX;
  for (unsigned int x = t; x != s; x = from[x]->from)
    ret = std::min(ret, from[x]->flow);
  for (unsigned int x = t; x != s; x = from[x]->from) {
    from[x]->flow -= ret;
    from[x]->rev->flow += ret;
  }
  return ret;
}
unsigned long long isap(const unsigned int n, const unsigned int s,
                        const unsigned int t)
{
  static const Edge* cur[maxv + 10];
  static unsigned int cnt[maxv + 10];
  bfs(t);
  std::copy(head, head + n, cur);
  for (unsigned int i = 0; i < n; ++i)
    ++cnt[dep[i]];
  unsigned long long ret = 0;
  unsigned int x = s;
  while (dep[s] < n) {
    if (x == t) {
      ret += augment(s, t);
      x = s;
    }
    bool adv = false;
    for (const Edge*& i = cur[x]; i; i = i->pre)
      if (i->flow && dep[i->to] + 1 == dep[x]) {
        adv = true;
        from[i->to] = i;
        x = i->to;
        break;
      }
    if (!adv) {
      unsigned int mnv = n;
      for (const Edge* i = head[x]; i; i = i->pre)
        if (i->flow) mnv = std::min(mnv, dep[i->to]);
      --cnt[dep[x]];
      if (!cnt[dep[x]]) break;
      dep[x] = mnv + 1;
      ++cnt[dep[x]];
      cur[x] = head[x];
      if (x != s) x = from[x]->from;
    }
  }
  return ret;
}

unsigned int r, c, offset;
struct Point {
  unsigned int r, c;
  inline bool operator==(const Point& other) const
  {
    return r == other.r && c == other.c;
  }
};
struct Step {
  int dr, dc;
};
struct PointHash {
  size_t operator()(const Point& s) const
  {
    return static_cast<unsigned long long>(s.r) * c + s.c;
  }
};
constexpr Step stepL[3] = {{-1, 0}, {1, 0}, {0, -1}};
constexpr Step stepR[3] = {{-1, 0}, {1, 0}, {0, 1}};
std::unordered_map<Point, unsigned int, PointHash> index;
enum class Type { Source, Sink };

inline Point operator+(const Point& a, const Step& b)
{
  return Point{int(a.r) + b.dr, int(a.c) + b.dc};
}
template <bool rev>
inline void linkVertex(const unsigned int x, const unsigned int y)
{
  if constexpr (rev)
    addEdge(y + offset, x, inf);
  else
    addEdge(x + offset, y, inf);
}
template <Type typ>
inline void linkST(const unsigned int id, const unsigned int st[])
{
  static bool linked[maxv + 10];
  if (linked[id]) return;
  if constexpr (typ == Type::Source)
    addEdge(st[static_cast<unsigned int>(Type::Source)], id, inf);
  else
    addEdge(id + offset, st[static_cast<unsigned int>(Type::Sink)], inf);
  linked[id] = true;
}
static void buildVertex(const unsigned int n)
{
  unsigned int acc = 0;
  for (const Cube* i = cub; i < cub + n; ++i) {
    index.insert({Point{i->r, i->c}, acc});
    addEdge(acc, acc + offset, i->w);
    ++acc;
  }
}
template <unsigned int rev>
inline void linkList(const unsigned int vert[], const unsigned int n,
                     const unsigned int cid, const unsigned int st[])
{
  static constexpr Type typ = rev ? Type::Sink : Type::Source;
  for (const unsigned int* i = vert; i < vert + n; ++i) {
    linkVertex<rev>(*i, cid);
    linkST<typ>(*i, st);
  }
}
template <bool rev>
static void linkKeyEdge(const Point p, const unsigned int st[])
{
  static const auto link = [](const unsigned int idx, const unsigned int idy) {
    addEdge(idx + offset, idy, inf);
  };
  const Point nextPoint{p.r, p.c + 1};
  unsigned int src[3], dest[3], next;
  unsigned int srcCnt = 0, destCnt = 0;
  if (const auto it = index.find(nextPoint); it != index.end())
    next = it->second;
  else
    return;
  for (unsigned int i = 0; i < 3; ++i) {
    if (const auto it = index.find(p + stepL[i]); it != index.end())
      src[srcCnt++] = it->second;
    if (const auto it = index.find(nextPoint + stepR[i]); it != index.end())
      dest[destCnt++] = it->second;
  }
  if (!srcCnt || !destCnt) return;
  const unsigned int cid = index.at(p);
  linkList<rev>(src, srcCnt, cid, st);
  linkList<!rev>(dest, destCnt, next, st);
  linkVertex<rev>(cid, next);
}
static std::tuple<unsigned int, unsigned int, unsigned int> buildGraph(
    const unsigned int r, const unsigned int c, const unsigned int n)
{
  offset = n;
  buildVertex(n);
  const unsigned int st[2] = {n * 2, n * 2 + 1};
  for (const Cube* i = cub; i < cub + n; ++i)
    if (i->r & 0x01) {
      if ((i->c & 0x03) == 1) linkKeyEdge<false>(Point{i->r, i->c}, st);
    }
    else if ((i->c & 0x03) == 3)
      linkKeyEdge<true>(Point{i->r, i->c}, st);
  return std::make_tuple(st[0], st[1], st[1] + 1);
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n;
  std::cin >> c >> r >> n;
  for (Cube* i = cub; i < cub + n; ++i)
    std::cin >> i->c >> i->r >> i->w;
  const auto [s, t, sz] = buildGraph(r, c, n);
  std::cout << isap(sz, s, t) << "\n";
  return 0;
}