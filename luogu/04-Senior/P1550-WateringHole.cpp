#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
using res_t = unsigned int;
constexpr int maxn = 300, maxm = (maxn * maxn) + maxn;

struct edge {
  unsigned int vx;
  unsigned int vy;
  res_t val;
} ed[maxm + 10], *cur = ed;
unsigned int fa[maxn + 10], siz[maxn + 10];

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
  siz[fx] += fy;
  return true;
}
res_t kruskal(const unsigned int n)
{
  res_t ret = 0;
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
  sort(ed, cur,
       [](const edge& a, const edge& b) -> bool { return a.val < b.val; });

  for (unsigned int i = 0; i < n; ++i) {
    static edge* e = ed;
    while (!TryMerge(e->vx, e->vy))
      ++e;
    ret += e->val;
    ++e;
  }
  return ret;
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n;
  cin >> n;

  for (unsigned int i = 1; i <= n; ++i, ++cur) {
    cin >> cur->val;
    cur->vx = 0;
    cur->vy = i;
  }
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= n; ++j) {
      cin >> cur->val;
      cur->vx = i;
      cur->vy = j;
      ++cur;
    }

  cout << kruskal(n) << endl;
  return 0;
}