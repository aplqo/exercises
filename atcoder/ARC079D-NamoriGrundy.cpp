#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <unordered_set>
const int maxn = 200000;

struct Edge {
  int to;
  const Edge *pre;
};
const Edge *head[maxn + 10];
int cycle[maxn + 10], *ptr = cycle;
int a[maxn + 10], degree[maxn + 10];
enum class State { NotVisit, Cycle, Visited, InStack, Last } vis[maxn + 10];
enum class DfsState { NotFound, InCycle, Found };

inline void addEdge(const int from, const int to)
{
  static Edge ed[maxn + 10], *cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  ++degree[from];
}
DfsState dfsCycle(const int x)
{
  vis[x] = State::InStack;
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (vis[to] == State::InStack) {
      vis[to] = State::Last;
      vis[x] = State::Cycle;
      *(ptr++) = x;
      return DfsState::InCycle;
    }
    else if (vis[to] == State::NotVisit)
      switch (dfsCycle(i->to)) {
        case DfsState::Found:
          return DfsState::Found;
        case DfsState::InCycle:
          *(ptr++) = x;
          if (vis[x] == State::Last) {
            vis[x] = State::Cycle;
            return DfsState::Found;
          }
          vis[x] = State::Cycle;
          return DfsState::InCycle;
        case DfsState::NotFound:
          break;
      }
  }
  vis[x] = State::Visited;
  return DfsState::NotFound;
}

inline int nextValid(int x, const std::unordered_set<int> &v)
{
  do {
    ++x;
  } while (v.count(x));
  return x;
}
void dfsVal(const int x)
{
  int miv = 0;
  std::unordered_set<int> child;
  child.reserve(degree[x]);
  for (const Edge *i = head[x]; i; i = i->pre) {
    if (vis[i->to] == State::Cycle) continue;
    dfsVal(i->to);
    child.insert(a[i->to]);
    if (miv == a[i->to]) miv = nextValid(miv, child);
  }
  a[x] = miv;
}

bool checkOddCycle()
{
  const int v = a[cycle[0]];
  for (const int *i = cycle; i < ptr; ++i)
    if (a[*i] != v) return true;
  return false;
}
bool check(const int n)
{
  for (int i = 1; i <= n; ++i)
    if (vis[i] == State::NotVisit && dfsCycle(i) == DfsState::Found) break;
  for (const int *i = cycle; i < ptr; ++i)
    dfsVal(*i);
  return !((ptr - cycle) & 0x01) || checkOddCycle();
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  for (int i = 1; i <= n; ++i) {
    int p;
    std::cin >> p;
    addEdge(p, i);
  }
  std::cout << (check(n) ? "POSSIBLE" : "IMPOSSIBLE") << "\n";
  return 0;
}