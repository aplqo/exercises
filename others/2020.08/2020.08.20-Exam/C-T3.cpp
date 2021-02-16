#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxl = 60;
const unsigned long long mod = 1e9 + 7;

unsigned long long f[maxl + 1][maxl + 1]
                    [maxl + 1];  // 2^i -> 2^l ~ 2^r (Must include 2^r)

static void init(const unsigned int n)
{
  for (unsigned int i = 0; i <= n; ++i)
    for (unsigned int j = 0; j <= i; ++j)
      f[i][j][i] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int l = 0; l <= i; ++l)
      for (unsigned int r = l; r <= i; ++r)
        for (unsigned int k = l; k <= r; ++k)
          f[i][l][r] =
              (f[i][l][r] + (f[i - 1][l][k] * f[i - 1][k][r]) % mod) % mod;
}
unsigned long long dp(const unsigned long long x)
{
  static unsigned long long a[maxl + 1][maxl + 1];
  memset(a, 0, sizeof(a));
  a[0][0] = x & 0x01 ? 1 : 0;
  for (unsigned int i = 1; i <= maxl; ++i) {
    for (unsigned int j = 0; j <= i; ++j) {
      if (!(x & (1ull << i)))
        a[i][j] = a[i - 1][j];
      else if (!(x & ((1ull << i) - 1)))
        a[i][j] = f[i][0][j];
      else {
        for (unsigned int k = 0; k <= j; ++k)
          a[i][j] = (a[i][j] + (a[i - 1][k] * f[i][k][j]) % mod) % mod;
      }
    }
  }
  unsigned long long ret = 0;
  for (unsigned long long i = 0; i <= maxl; ++i)
    ret = (ret + a[maxl][i]) % mod;
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  init(maxl);
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned long long n;
    cin >> n;
    cout << dp(n) << "\n";
  }
  return 0;
}