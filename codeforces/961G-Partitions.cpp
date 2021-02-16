#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxn = 2e5;
constexpr unsigned long long mod = 1e9 + 7;

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
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

  unsigned long long val;
};
Number fac[maxn + 10], ifac[maxn + 10];

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
    ifac[i].val = mod - (ifac[mod % i] * Number{mod / i}).val;
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
  return fac[n] * ifac[m] * ifac[n - m];
}
Number stirling(const unsigned int n, const unsigned int m)
{
  Number ret{0};
  for (unsigned int i = 0; i <= m; ++i) {
    const Number p = C(m, i) * quickPow(Number{i}, n);
    if ((m - i) & 0x01)
      ret += Number{mod - p.val};
    else
      ret += p;
  }
  return ret * ifac[m];
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, k;
  std::cin >> n >> k;
  initFactor(n);
  Number w{0};
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int a;
    std::cin >> a;
    w += Number{a};
  }
  std::cout << w * (stirling(n, k) + Number{n - 1} * stirling(n - 1, k))
            << "\n";
  return 0;
}