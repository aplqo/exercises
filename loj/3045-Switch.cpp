#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
const int maxn = 100, maxsp = 5e4;
constexpr unsigned int mod = 998244353, inv2 = 499122177;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned int ret = l.val + r.val;
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
    return Number{
        static_cast<unsigned int>(((unsigned long long)l.val * r.val) % mod)};
  }
  inline void operator*=(const Number r)
  {
    val = (static_cast<unsigned long long>(val) * r.val) % mod;
  }

  unsigned int val;
};

Number quickPow(Number a, unsigned int e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number x) { return quickPow(x, mod - 2); }

Number fMem[maxsp * 2 + 10], gMem[maxsp * 2 + 10];
Number *const f = fMem + maxsp + 5, *const g = gMem + maxsp + 5;
int p[maxn + 10], s[maxn + 10];

template <bool negitive>
inline void shift(const Number src[], Number dest[], const int n,
                  const int offset)
{
  if (!negitive) {
    std::memcpy(dest + offset, src, sizeof(src[0]) * (n - offset));
    std::memset(dest, 0, sizeof(dest[0]) * offset);
  }
  else {
    std::memcpy(dest, src + offset, sizeof(src[0]) * (n - offset));
    std::memset(dest + (n - offset), 0, sizeof(dest[0]) * offset);
  }
}
/// @brief dest*x^v +\- dest*x^{-v}
template <bool neg>
void multiply(Number dest[], const int sumP, const int v)
{
  static Number mema[maxsp * 2 + 10], memb[maxsp * 2 + 10];
  static Number *const tmpa = mema + maxsp + 5, *const tmpb = memb + maxsp + 5;
  const int n = sumP * 2 + 1;
  shift<false>(dest - sumP, tmpa - sumP, n, v);
  shift<true>(dest - sumP, tmpb - sumP, n, v);
  for (int i = -sumP; i <= sumP; ++i)
    if constexpr (neg)
      dest[i] = (tmpa[i] - tmpb[i]) * Number{inv2};
    else
      dest[i] = (tmpa[i] + tmpb[i]) * Number{inv2};
}

Number solve(const int n)
{
  const int sumP = std::accumulate(p, p + n, 0);
  f[0].val = g[0].val = 1;
  for (int i = 0; i < n; ++i) {
    if (s[i])
      multiply<true>(f, sumP, p[i]);
    else
      multiply<false>(f, sumP, p[i]);
    multiply<false>(g, sumP, p[i]);
  }
  Number acc{0};
  for (int i = -sumP; i <= sumP; ++i)
    acc += (f[i] - g[i]) *
           inverse(Number{(i + static_cast<int>(mod) - sumP) % mod});
  return acc * quickPow(Number{2}, n) * Number{static_cast<unsigned int>(sumP)};
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  std::copy_n(std::istream_iterator<int>(std::cin), n, s);
  std::copy_n(std::istream_iterator<int>(std::cin), n, p);
  std::cout << solve(n).val << "\n";
  return 0;
}