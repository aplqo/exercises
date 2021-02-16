#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 500000;

unsigned int fa[maxn + 1], val[maxn + 1], siz[maxn + 1], dep[maxn + 1];

unsigned int GetFather(unsigned int x)
{
  if (fa[x] == x) return x;
  int p = fa[x], ret = GetFather(fa[x]);
  dep[x] = dep[p] + 1;
  return ret;
}
void Merge(unsigned int x, unsigned int y, unsigned int nu)
{
  int fx = GetFather(x), fy = GetFather(y);
  if (fx == fy) return;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  val[fy] = nu;
  dep[fy] = dep[fx] + 1;
  siz[fx] = max(siz[fx], siz[fy] + 1);
}
unsigned int Solve(unsigned int x, unsigned int y)
{
  if (GetFather(x) != GetFather(y)) return 0;
  unsigned int ans = 0, l = x, r = y;
  while (l != r) {
    if (dep[l] > dep[r]) {
      ans = max(ans, val[l]);
      l = fa[l];
    }
    else {
      ans = max(ans, val[r]);
      r = fa[r];
    }
  }
  return ans;
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, m, lans = 0;
  cin >> n >> m;
  iota(fa, fa + n + 1, 0);
  fill(siz, siz + 1 + n, 1);
  for (unsigned int i = 0; i < m; ++i) {
    static unsigned int lst = 0;
    unsigned int op, v, u;
    cin >> op >> u >> v;
    v ^= lans;
    u ^= lans;
    if (op) {
      lans = Solve(v, u);
      cout << lans << endl;
    }
    else
      Merge(v, u, ++lst);
  }
  return 0;
}