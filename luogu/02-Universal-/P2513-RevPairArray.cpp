#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000, maxk = 1000;
constexpr unsigned long long mod = 10000;

unsigned long long dp(const unsigned int n, const unsigned int k)
{
  static unsigned long long f[maxn + 1][maxk + 1];
  static unsigned long long sum[maxk + 1];
  f[1][0] = 1;
  fill(sum, sum + k + 1, 1);
  for (unsigned int i = 2; i <= n; ++i) {
    f[i][0] = 1;
    for (unsigned int j = 1; j <= k; ++j) {
      const unsigned int l = j - min(j, i - 1);
      f[i][j] = (sum[j] + mod - (l ? sum[l - 1] : 0)) % mod;
    }
    sum[0] = 1;
    for (unsigned int t = 1; t <= k; ++t)
      sum[t] = (sum[t - 1] + f[i][t]) % mod;
  }
  return f[n][k];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  cout << dp(n, k) << endl;
  return 0;
}