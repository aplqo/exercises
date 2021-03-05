#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <bitset>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
const int maxn = 5e4, maxm = 1e5, maxt = 1e6;
constexpr int maxv = 2 * (maxn + maxm), maxe = 2 * (maxv + maxm);
constexpr int block = 25000;
using Bitset = std::bitset<block>;

int virtVertex;
class VertexMap {
 public:
  inline void create(const int t)
  {
    const auto [it, suc] = mp.insert({t, 0});
    if (suc) it->second = virtVertex++;
  }
  inline int operator[](const int t) const { return mp.lower_bound(t)->second; }
  inline int end() const { return mp.crbegin()->second; }
  inline void link() const;

 private:
  std::map<int, int> mp;
} people[maxn + 10];
struct Edge {
  int to;
  const Edge *pre;
} ed[maxe + 10];
const Edge *head[maxv + 10];
int inDegree[maxv + 10];

struct Predict {
  int type, time, x, y;
} predict[maxm + 10];
int ans[maxn + 10];
bool mustDead[maxn + 10];

template <bool v>
inline int var(const int x)
{
  return (x << 1) | v;
}
inline int notVar(const int x) { return x ^ 1; }
template <bool v>
inline int endVar(const int x)
{
  return var<v>(people[x].end());
}

/// @brief add reversed edge
inline void addHalfEdge(const int to, const int from)
{
  static Edge *cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  ++inDegree[to];
}
inline void implies(const int x, const int y)
{
  addHalfEdge(x, y);
  addHalfEdge(notVar(y), notVar(x));
}
template <bool v>
inline void ifThen(const int varX, const int varY)
{
  implies(var<true>(varX), var<v>(varY));
}

inline void printGraph()
{
  static const auto chk = [](const int x) { return x & 0x01 ? "T" : "F"; };
  for (int i = 0; i < virtVertex * 2; ++i)
    for (const Edge *j = head[i]; j; j = j->pre) {
      std::clog << (j->to >> 1) << chk(j->to) << " -> " << (i >> 1) << chk(i)
                << "\n";
    }
  std::clog.flush();
}

inline void VertexMap::link() const
{
  const auto end = std::prev(mp.cend());
  for (auto it = mp.cbegin(); it != end;) {
    const auto nxt = std::next(it);
    implies(var<false>(it->second), var<false>(nxt->second));
    it = nxt;
  }
}
static void buildGraph(const int n, const int m, const int t)
{
  for (VertexMap *i = people; i < people + n; ++i)
    i->create(t + 1);
  for (const Predict *i = predict; i < predict + m; ++i)
    people[i->x].create(i->time);

  for (const Predict *i = predict; i < predict + m; ++i)
    if (i->type)
      ifThen<false>(people[i->x][i->time], people[i->y][i->time]);
    else
      implies(var<false>(people[i->x][i->time]),
              var<false>(people[i->y][i->time + 1]));
  for (const VertexMap *i = people; i < people + n; ++i)
    i->link();
}
void topological(const int n, const int begin, const int end)
{
  static int degree[maxv + 10];
  static Bitset reach[maxv + 10], dead;
  std::queue<int> q;
  std::copy(inDegree, inDegree + virtVertex * 2, degree);
  for (Bitset *i = reach; i < reach + virtVertex * 2; ++i)
    i->reset();
  dead.reset();

  for (int i = begin; i < end; ++i)
    reach[endVar<false>(i)].set(i - begin);
  for (int i = 0; i < virtVertex * 2; ++i)
    if (!degree[i]) q.push(i);
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    for (const Edge *i = head[cur]; i; i = i->pre) {
      --degree[i->to];
      reach[i->to] |= reach[cur];
      if (!degree[i->to]) q.push(i->to);
    }
  }
  for (int i = begin; i < end; ++i)
    if (reach[endVar<true>(i)].test(i - begin)) {
      dead.set(i - begin);
      mustDead[i] = true;
    }
  for (int i = 0; i < n; ++i) {
    const int v = endVar<true>(i);
    reach[v] |= dead;
    ans[i] += end - begin - reach[v].count();
  }
}
void countAlive(const int n)
{
  int i;
  for (i = 0; i + block <= n; i += block)
    topological(n, i, i + block);
  if (i < n) topological(n, i, n);
  for (int i = 0; i < n; ++i)
    ans[i] = mustDead[i] ? 0 : ans[i] - 1;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int t, n, m;
  std::cin >> t >> n >> m;
  for (Predict *i = predict; i < predict + m; ++i) {
    std::cin >> i->type >> i->time >> i->x >> i->y;
    --(i->x);
    --(i->y);
  }
  buildGraph(n, m, t);

  // printGraph();

  countAlive(n);
  std::copy(ans, ans + n, std::ostream_iterator<int>(std::cout, " "));
  std::cout.put('\n');
  return 0;
}
