#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
const unsigned int maxn = 200000;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int v[maxn + 19];
std::multiset<unsigned int> f[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
void dfs(const unsigned int x)
{
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    dfs(to);
    if (f[to].size() > f[x].size()) f[x].swap(f[to]);
    f[x].merge(f[to]);
  }
  if (auto it = f[x].lower_bound(v[x]); it != f[x].begin()) f[x].erase(--it);
  f[x].insert(v[x]);
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned int n;
  std::cin >> n;
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, v + 1);
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int p;
    std::cin >> p;
    addEdge(p, i);
  }
  dfs(1);
  std::cout << f[1].size() << "\n";
  return 0;
}