#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxl = 32;

unsigned int f[maxl + 10][maxl + 10], cnt[maxl + 1];

static void initF(const unsigned int n)
{
  f[0][1] = f[0][0] = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    f[i][0] = 1;
    for (unsigned int j = 1; j <= i + 1; ++j)
      f[i][j] = f[i - 1][j - 1] + f[i - 1][j];
  }
  for (unsigned int i = 0; i <= n; ++i)
    partial_sum(f[i], f[i] + n + 2, f[i]);
  cnt[0] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    cnt[i] = f[i][(i + 1) >> 1] - f[i - 1][(i + 1) >> 1] + cnt[i - 1];
}
inline unsigned int toBits(unsigned int v, unsigned int res[])
{
  unsigned int pos = 0;
  do {
    res[pos++] = v & 0x01;
    v >>= 1;
  } while (v);
  return pos;
}
unsigned int dp(const unsigned int v)
{
  unsigned int digits[maxl + 10];
  const unsigned int len = toBits(v, digits);
  if (len == 1) return v ? 1 : 0;
  unsigned int cnt1 = 1, ret = cnt[len - 2];
  for (int i = len - 2; i >= 0 && cnt1 * 2 <= len; --i)
    if (digits[i]) {
      ret += i ? f[i - 1][len / 2 - cnt1] : 1;
      ++cnt1;
    }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  initF(maxl);
  unsigned int l, r;
  cin >> l >> r;
  cout << dp(r + 1) - dp(l) << "\n";
  return 0;
}