#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const int maxn = 2e5;

struct Edge {
  int to, length;
  mutable bool removed;
  const Edge *pre, *rev;
} ed[maxn * 2 + 10];
const Edge *head[maxn + 10];
enum class VisState {
  NotVisit,
  Last,
  Cycle,
  InStack,
  Visited
} visit[maxn + 10];

enum { Prev, Next };
struct CycleVertex {
  int id;
  long long path[2], vertex[2], depth;
  const Edge *edge;

  inline long long maxDistance() const
  {
    return std::max({path[Prev], path[Next], vertex[Prev] + vertex[Next]});
  }
} cycle[maxn + 10], *end = cycle;

inline void addEdge(const int from, const int to, const int length)
{
  static Edge *cur = ed;
  new (cur) Edge{to, length, false, head[from], cur + 1};
  head[from] = cur++;
  new (cur) Edge{from, length, false, head[to], cur - 1};
  head[to] = cur++;
}

enum class DFSState { NotFound, InCycle, Found };
DFSState dfsCycle(const int x, const int f)
{
  static const auto addVertex = [](const int x, const Edge *e) {
    end->id = x;
    (end++)->edge = e;
    visit[x] = VisState::Cycle;
  };
  visit[x] = VisState::InStack;
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    if (visit[to] == VisState::InStack) {
      addVertex(x, i);
      visit[to] = VisState::Last;
      return DFSState::InCycle;
    }
    else
      switch (dfsCycle(i->to, x)) {
        case DFSState::Found:
          return DFSState::Found;
        case DFSState::InCycle:
          if (visit[x] == VisState::Last) {
            addVertex(x, i);
            return DFSState::Found;
          }
          addVertex(x, i);
          return DFSState::InCycle;
        case DFSState::NotFound:
          break;
      }
  }
  return DFSState::NotFound;
}

struct ValFrom {
  long long val;
  int from;
  inline bool operator<(const ValFrom &r) const { return val < r.val; }
  inline ValFrom operator+(const int d) const { return ValFrom{val + d, from}; }
};

enum class Ignore { None, Removed, Cycle };
template <Ignore ign>
ValFrom dfsFarthest(const int x, const int f)
{
  ValFrom ret{0, x};
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    if constexpr (ign == Ignore::Removed) {
      if (i->removed) continue;
    }
    else if constexpr (ign == Ignore::Cycle)
      if (visit[to] == VisState::Cycle) continue;
    ret = std::max(ret, dfsFarthest<ign>(to, x) + i->length);
  }
  return ret;
}
long long diameter()
{
  const ValFrom v1 = dfsFarthest<Ignore::Removed>(1, 0);
  return dfsFarthest<Ignore::Removed>(v1.from, 0).val;
}

template <int d>
void cycleInit(CycleVertex *begin, CycleVertex *end)
{
  constexpr int step = d == Prev ? 1 : -1;
  long long lastEdge = 0, cycle = 0;
  long long chain = 0, path = 0;
  long long dist = 0;
  for (CycleVertex *i = begin; i != end; i += step) {
    if constexpr (d == Prev) {
      i->vertex[Prev] = dist;
      i->path[Prev] = path;
      lastEdge = i->edge->length;
    }
    cycle += lastEdge;
    chain += lastEdge;
    path = std::max(path, i->depth + chain);
    chain = std::max(chain, i->depth);
    dist = std::max(dist, i->depth + cycle);
    if constexpr (d == Next) {
      i->vertex[Next] = dist;
      i->path[Next] = path;
      lastEdge = i->edge->length;
    }
  }
}
CycleVertex *findEdge()
{
  long long v = cycle->maxDistance();
  CycleVertex *ret = cycle;
  for (CycleVertex *i = cycle; i != end; ++i)
    if (long long p = i->maxDistance(); p < v) {
      ret = i;
      v = p;
    }
  return ret;
}
void removeEdge()
{
  for (CycleVertex *i = cycle; i != end; ++i)
    i->depth = dfsFarthest<Ignore::Cycle>(i->id, 0).val;
  cycleInit<Prev>(cycle, end);
  cycleInit<Next>(end - 1, cycle - 1);
  const Edge *e = findEdge()->edge;
  e->removed = e->rev->removed = true;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    int u, v, l;
    std::cin >> u >> v >> l;
    addEdge(u, v, l);
  }
  dfsCycle(1, 0);
  removeEdge();
  std::cout << diameter() << "\n";
  return 0;
}