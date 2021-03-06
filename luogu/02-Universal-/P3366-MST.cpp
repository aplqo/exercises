#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 5000, maxm = 2e5;
using num_t = int;

unsigned int fa[maxn + 10], siz[maxn + 10];
struct edge {
  unsigned int x, y;
  num_t z;
} ed[maxm + 10];

unsigned int GetFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
inline bool TryMerge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = GetFather(x), fy = GetFather(y);
  if (fx == fy) return false;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
  return true;
}
num_t kruskal(const int n, const int m)
{
  num_t ans = 0;
  unsigned int se = 0;
  iota(fa, fa + n + 1, 0);
  fill(siz, siz + n + 1, 1);
  sort(ed, ed + m, [](const edge& a, const edge& b) { return a.z < b.z; });

  for (edge* i = ed; i < ed + m; ++i) {
    for (; !TryMerge(i->x, i->y) && i < ed + m; ++i)
      ;
    if (i < ed + m) {
      ans += i->z;
      ++se;
    }
  }
  if (siz[GetFather(1)] != n || se != n - 1) return -1;
  return ans;
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, m;
  num_t ans;
  cin >> n >> m;
  for (edge* i = ed; i < ed + m; ++i)
    cin >> i->x >> i->y >> i->z;
  ans = kruskal(n, m);
  if (ans != -1)
    cout << ans << endl;
  else
    cout << "orz" << endl;
  return 0;
}