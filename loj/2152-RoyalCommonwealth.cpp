#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
const unsigned int maxn = 1000;

struct Edge {
  int to;
  const Edge *pre;
} ed[maxn * 2 + 10];
const Edge *head[maxn + 10];
struct City {
  int *begin, *end;
  int capital;
} city[maxn + 10];
int cid, b;
int stk[maxn + 10], *top = stk, mem[maxn + 10], *cur = mem;

inline void addEdge(const int from, const int to)
{
  static Edge *cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}
void addCity(const int cap, int *const old)
{
  city[cid].begin = cur;
  for (const int *i = old; i < top; ++i)
    *(cur++) = *i;
  city[cid].end = cur;
  city[cid].capital = cap;
  ++cid;
  top = old;
}
void dfs(const int x, const int f)
{
  *(top++) = x;
  for (const Edge *i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (to == f) continue;
    int *const old = top;
    dfs(to, x);
    if (top - old >= b) addCity(x, old);
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n >> b;
  for (int i = n - 1; i; --i) {
    int a, b;
    std::cin >> a >> b;
    addEdge(a, b);
  }
  dfs(1, 0);
  if (top != stk) addCity(1, stk);
  std::cout << cid << "\n";
  for (const City *i = city; i < city + cid; ++i) {
    std::copy(i->begin, i->end, std::ostream_iterator<int>(std::cout, " "));
    std::cout.put('\n');
  }
  for (const City *i = city; i < city + cid; ++i)
    std::cout << i->capital << " ";
  std::cout.put('\n');
  return 0;
}
