#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const int maxn = 2000, maxm = 2000, maxk = 2000;
const unsigned int mod = 1e9 + 7;

unsigned int dp(const int n, const int m, const int k)
{
  static const auto add = [](unsigned int a, unsigned int b) {
    a += b;
    return a >= mod ? a - mod : a;
  };
  static const auto minus = [](unsigned int a, unsigned int b) {
    return a >= b ? a - b : a + mod - b;
  };
  static unsigned int f[maxn * 2 + 10][maxn + 10][2];
  const int nMod = n % (k - 1), mMod = (m + k - 1 - 1) % (k - 1);
  const int len = (n + m - 1) / (k - 1);
  unsigned int ret = 0;

  f[1][0][0] = 1;
  for (int i = 1; i < k; ++i) {
    f[1][i][1] = 1;
    if (k - 1 - i == mMod) ++ret;
  }

  for (int i = 2; i <= len; ++i) {
    static unsigned int sum[maxn + 10];

    const int sumK = (k - 1) * i;
    sum[0] = add(f[i - 1][0][0], f[i - 1][0][1]);
    f[i][0][0] = 1;
    for (int j = 1; j <= n; ++j) {
      f[i][j][0] = add(f[i - 1][j][0], f[i - 1][j][1]);
      sum[j] = add(sum[j - 1], f[i][j][0]);
      f[i][j][1] = j >= k ? minus(sum[j - 1], sum[j - k]) : sum[j - 1];

      if (j % (k - 1) == nMod && (sumK - j) <= m - 1 &&
          (sumK - j) % (k - 1) == mMod)
        ret = add(ret, f[i][j][1]);
    }
  }

  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m, k;
  std::cin >> n >> m >> k;
  std::cout << dp(n, m, k) << "\n";
  return 0;
}