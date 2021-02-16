#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 100, maxm = 2000;
constexpr unsigned long long mod = 998244353;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline Number& operator x##=(const Number& r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  friend inline istream& operator>>(istream& is, Number& v)
  {
    return is >> v.val;
  }
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
Number sum[maxn + 1], a[maxn + 1][maxm + 1];

Number countNoRestrict(const unsigned int n)
{
  Number ret = 1;
  for (unsigned int i = 1; i <= n; ++i)
    ret *= (sum[i] + 1);
  return ret - 1;
}
Number countInvFood(const unsigned int n, const unsigned int t)
{
  static Number f[maxn + 1][maxn * 4 + 1];
  static const auto index = [](const unsigned int col,
                               const int val) -> Number& {
    return f[col][val + maxn * 2];
  };
  index(0, 0) = 1;
  for (int i = 1; i <= n; ++i) {
    fill(f[i], f[i] + maxn * 4, 0);
    for (int j = -i; j <= i; ++j)
      index(i, j) = index(i - 1, j) + index(i - 1, j - 1) * a[i][t] +
                    index(i - 1, j + 1) * (sum[i] - a[i][t]);
  }
  return accumulate(f[n] + maxn * 2 + 1, f[n] + maxn * 2 + n + 1, Number(0));
}
Number countInvalid(const unsigned int n, const unsigned int m)
{
  static Number g[maxn + 1];
  Number ret = 0;
  for (unsigned int i = 1; i <= m; ++i)
    ret += countInvFood(n, i);
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= m; ++j) {
      cin >> a[i][j];
      sum[i] += a[i][j];
    }
  cout << countNoRestrict(n) - countInvalid(n, m) << "\n";
  return 0;
}
