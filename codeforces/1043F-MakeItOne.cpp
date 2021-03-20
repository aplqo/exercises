#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const int maxn = 3e5, maxa = 3e5, maxSet = 7;
const unsigned int mod = 1e9 + 7;

struct Number {
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{static_cast<unsigned int>(
        (static_cast<unsigned long long>(l.val) * r.val) % mod)};
  }
  inline void operator*=(const Number r)
  {
    val = (static_cast<unsigned long long>(val) * r.val) % mod;
  }

  unsigned int val;
};

int cnt[maxn + 10], multiply[maxn + 10];
Number fac[maxn + 10], invFac[maxn + 10];

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
static void initFactor(const unsigned int n)
{
  fac[0].val = fac[1].val = 1;
  invFac[0].val = invFac[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    fac[i] = fac[i - 1] * Number{i};
  invFac[n] = inverse(fac[n]);
  for (unsigned int i = n; i; --i)
    invFac[i - 1] = invFac[i] * Number{i};
}
static void initMultiply(const int n)
{
  for (int i = 1; i <= n; ++i)
    for (int j = i; j <= n; j += i)
      multiply[i] += cnt[j];
}
inline Number C(const int n, const int m)
{
  return n >= m ? fac[n] * invFac[m] * invFac[n - m] : Number{0};
}
int dp(const int ma)
{
  for (int i = 1; i <= maxSet; ++i) {
    static Number f[maxa + 10];
    for (int j = ma; j; --j) {
      f[j] = C(multiply[j], i);
      for (int k = j + j; k <= ma; k += j)
        f[j] -= f[k];
    }
    if (f[1].val) return i;
  }
  return -1;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, ma = 0;
  std::cin >> n;
  for (int i = 0; i < n; ++i) {
    int a;
    std::cin >> a;
    ++cnt[a];
    ma = std::max(ma, a);
  }
  initMultiply(ma);
  initFactor(n);
  std::cout << dp(ma) << "\n";
  return 0;
}