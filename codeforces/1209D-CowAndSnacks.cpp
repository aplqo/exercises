#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1e5;

int fa[maxn + 5], siz[maxn + 5], sel[maxn + 5];
int n, k, ans;

inline void init(int n)
{
  iota(fa, fa + n + 1, 0);
  // fill(siz, siz + n + 1, 1);
}
int GetFather(int x) { return fa[x] == x ? x : fa[x] = GetFather(fa[x]); }
inline void merge(int x, int y)
{
  int fx = GetFather(x), fy = GetFather(y);
  if (fx == fy) return;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy] + 1;
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> k;
  init(n);

  for (int i = 0; i < k; ++i) {
    int x, y;
    cin >> x >> y;
    sel[x] = true;
    sel[y] = true;
    merge(x, y);
  }
  for (int i = 1; i <= n; ++i)
    if (fa[i] == i && sel[i]) ans += siz[i];
  ans = k - ans;
  cout << ans << endl;
  return 0;
}