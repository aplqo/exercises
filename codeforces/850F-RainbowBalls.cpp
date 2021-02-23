#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <iostream>
const int maxn = 2500, maxa = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

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

static Number solve(const int n, const int maxA, const int sumA)
{
  static Number f[maxa + 10];
  Number acc = inverse(Number{sumA});
  for (int i = 1; i <= maxA; ++i) {
    f[i] = Number{sumA - 1} * Number{sumA - i} * acc;
    acc += inverse(Number{sumA - i});
  }
  if (sumA <= maxA) f[sumA].val = 0;
  Number ret{};
  for (const int *i = a; i < a + n; ++i)
    ret += f[*i];
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, maxA = 0, sumA = 0;
  std::cin >> n;
  for (int *i = a; i < a + n; ++i) {
    std::cin >> *i;
    maxA = std::max(maxA, *i);
    sumA += *i;
  }
  std::cout << solve(n, maxA, sumA).val << "\n";
  return 0;
}