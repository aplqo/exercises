#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxm = 110;

int fa[maxm], siz[maxm];
bool sel[maxm];
int n, m, ans;

int GetFather(int x) { return fa[x] == x ? x : fa[x] = GetFather(fa[x]); }
inline void merge(int x, int y)
{
  int fx = GetFather(x), fy = GetFather(y);
  if (fx == fy) return;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin >> n >> m;
  iota(fa, fa + m + 1, 0);
  fill(siz, siz + m + 1, 1);
  for (int i = 0; i < n; ++i) {
    int k;
    cin >> k;
    if (k) {
      int lst, cur;
      cin >> lst;
      sel[lst] = true;
      for (int j = 1; j < k; ++j) {
        cin >> cur;
        sel[cur] = true;
        merge(lst, cur);
      }
    }
    else
      ++ans;
  }
  for (int i = 1; i <= m; ++i)
    GetFather(i);
  sort(fa + 1, fa + m + 1);
  int v = unique(fa + 1, fa + m + 1) - (fa + 1) - 1 -
          count(sel + 1, sel + m + 1, false);
  if (v > 0) ans += v;
  cout << ans << endl;
  return 0;
}