#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 50, maxl = 60;

unsigned long long a[maxn + 1];

unsigned long long solve(const unsigned int n, const unsigned int k)
{
  static bool f[maxn + 1][maxn + 1];
  unsigned long long ret = 0;
  for (int i = maxl; i >= 0; --i) {
    const unsigned long long msk = 1ull << i;
    memset(f, 0, sizeof(f));
    f[0][0] = true;
    for (unsigned int i = 1; i <= n; ++i) {
      const unsigned int mk = min(i, k);
      for (unsigned int j = 1; j <= mk; ++j)
        for (unsigned int p = 0; p < i && !f[i][j]; ++p)
          f[i][j] = f[i][j] || (f[p][j - 1] && ((a[i] - a[p]) & msk) &&
                                ((a[i] - a[p]) & ret) == ret);
    }
    if (f[n][k]) ret |= msk;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned long long v;
    cin >> v;
    a[i] = a[i - 1] + v;
  }
  cout << solve(n, k) << "\n";
  return 0;
}