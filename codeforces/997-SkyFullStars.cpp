#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxn = 1000000;
constexpr unsigned long long mod = 998244353, inv3 = 464040534;

class Number {
 public:
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return Number{(l.val x r.val) % mod};                         \
  }                                                               \
  inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
  decl(+);
  decl(*);
#undef decl
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }
  inline constexpr Number operator-() const { return Number{mod - val}; }

  unsigned long long val;
};

Number fac[maxn + 10], ifac[maxn + 10];

inline constexpr Number operator""_N(const unsigned long long p)
{
  return Number{p};
}
Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
static void initModularInv(const unsigned int n)
{
  ifac[0].val = ifac[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    ifac[i] = -ifac[mod % i] * Number{mod / i};
}
static void initFactor(const unsigned int n)
{
  initModularInv(n);
  fac[0].val = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    fac[i] = fac[i - 1] * Number{i};
    ifac[i] *= ifac[i - 1];
  }
}

inline Number C(const unsigned int n, const unsigned int m)
{
  return fac[n] * ifac[n - m] * ifac[m];
}
Number sumZero(const unsigned long long n)
{
  Number ret{0};
  for (unsigned int i = 1; i <= n; ++i)
    ret += (i & 0x01 ? 2_N : -2_N) * C(n, i) * quickPow(3_N, n * (n - i) + i);
  return ret;
}
Number sumPositive(const unsigned int n)
{
  Number ret{0}, acc{mod - 1};
  for (unsigned int i = 0; i < n; ++i) {
    ret += (i & 0x01 ? 3_N : -3_N) * C(n, i) *
           (quickPow(acc + 1_N, n) - quickPow(acc, n));
    acc *= 3_N;
  }
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n;
  std::cin >> n;
  initFactor(n);
  std::cout << sumPositive(n) + sumZero(n) << "\n";
  return 0;
}