#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxn = 100000, maxsa = 300000;
constexpr unsigned long long mod = 998244353;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned long long ret = l.val + r.val;
    return Number{ret >= mod ? ret - mod : ret};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{(l.val * r.val) % mod};
  }
  inline void operator*=(const Number r) { val = (val * r.val) % mod; }

  unsigned long long val;
};
int a[maxn + 10];
Number f[maxsa + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number x) { return quickPow(x, mod - 2); }

static void init(const int n, const int sa)
{
  const Number ns{sa};
  f[0].val = f[1].val = 1;
  for (int i = 2; i <= sa; ++i) {
    const Number na{i - 1};
    f[i] = ((Number{sa} + na * Number{n - 2}) * f[i - 1] -
            na * Number{n - 1} * (f[i - 2] + Number{1})) *
           inverse(ns - na);
  }
}
Number solve(const int n, const int sa)
{
  Number ret{0};
  for (const int* i = a; i < a + n; ++i)
    ret += f[*i];
  return ret - (f[sa] + f[0] * Number{n - 1});
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, sa = 0;
  std::cin >> n;
  for (int* i = a; i < a + n; ++i) {
    std::cin >> *i;
    sa += *i;
  }
  init(n, sa);
  std::cout << solve(n, sa).val << "\n";
  return 0;
}