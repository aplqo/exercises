#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000;
constexpr unsigned long long mod = 1e9 + 7;

class Number {
 public:
  Number(const unsigned long long v = 0) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r) { *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val;
};
Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
Number gf[2][maxn + 10][maxn + 10][2], gsum[2][maxn + 10][maxn + 10][2];

template <bool start>
static void dpChain(const unsigned int n, const unsigned int m,
                    const unsigned int a, const unsigned int b)
{
  Number(*const f)[maxn + 10][2] = gf[start],
                          (*const sum)[maxn + 10][2] = gsum[start];
  if (start) {
    for (unsigned int i = 1; i <= m && i <= b; ++i)
      sum[0][i][1] = f[0][i][1] = 1;
  }
  else {
    for (unsigned int i = 1; i <= n && i <= a; ++i)
      sum[i][0][0] = f[i][0][0] = 1;
  }
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= m; ++j) {
      f[i][j][1] = sum[i][j - 1][0] - (j > b ? sum[i][j - b - 1][0] : 0);
      sum[i][j][1] = sum[i - 1][j][1] + f[i][j][1];
      f[i][j][0] = sum[i - 1][j][1] - (i > a ? sum[i - a - 1][j][1] : 0);
      sum[i][j][0] = sum[i][j - 1][0] + f[i][j][0];
    }
}
Number dp(const unsigned int n, const unsigned int m, const unsigned int a,
          const unsigned int b)
{
  dpChain<0>(n, m, a, b);
  dpChain<1>(n, m, a, b);
  Number ret = 0;
  for (unsigned int i = 1; i <= n && i <= a; ++i)
    ret += gf[1][n - i][m][1] * i;
  for (unsigned int j = 1; j <= m && j <= b; ++j)
    ret += gf[0][n][m - j][0] * j;
  return ret;
}
Number factor(const unsigned int n)
{
  Number ret = 1;
  for (unsigned int i = 1; i <= n; ++i)
    ret *= i;
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, a, b;
  cin >> n >> m >> a >> b;
  if (a >= n && b >= m) {
    cout << factor(n + m) * quickPow(factor(n) * factor(m), mod - 2) << "\n";
    return 0;
  }
  cout << dp(n, m, a, b) << "\n";
  return 0;
}