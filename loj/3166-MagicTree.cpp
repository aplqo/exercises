#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5;
typedef map<unsigned int, unsigned long long> Map;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn + 10];
const Edge* head[maxn + 10];
struct Fruit {
  bool exist = false;
  unsigned int day, juice;
} fru[maxn + 10];
Map heavest[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
void join(Map& dest, Map& src)
{
  if (dest.size() < src.size()) swap(dest, src);
  for (const auto [day, val] : src)
    dest[day] += val;
}
void correctFruit(Map& mp, const Map::iterator vp,
                  const unsigned long long rootJuice)
{
  unsigned long long acc = rootJuice;
  while (acc) {
    const auto it = next(vp);
    if (it == mp.end()) break;
    const unsigned long long v = min(acc, it->second);
    acc -= v;
    it->second -= v;
    if (!it->second) mp.erase(it);
  }
}

void dfs(const unsigned int x)
{
  for (const Edge* i = head[x]; i; i = i->pre) {
    dfs(i->to);
    join(heavest[x], heavest[i->to]);
  }
  if (fru[x].exist) {
    const Fruit f = fru[x];
    const auto it = heavest[x].lower_bound(f.day);
    if (it == heavest[x].end())
      heavest[x][f.day] = f.juice;
    else if (it->first == f.day) {
      it->second += f.juice;
      correctFruit(heavest[x], it, f.juice);
    }
    else
      correctFruit(heavest[x], heavest[x].insert({f.day, f.juice}).first,
                   f.juice);
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, k;
  cin >> n >> m >> k;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int p;
    cin >> p;
    addEdge(p - 1, i);
  }
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int p;
    cin >> p;
    --p;
    fru[p].exist = true;
    cin >> fru[p].day >> fru[p].juice;
  }
  dfs(0);
  cout << accumulate(heavest[0].begin(), heavest[0].end(), 0ull,
                     [](unsigned long long ac, Map::value_type v) {
                       return ac + v.second;
                     })
       << "\n";
  return 0;
}