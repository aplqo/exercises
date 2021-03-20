#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
const int maxn = 1e5;
const unsigned int mod = 998244353;

struct Number {
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline void operator*=(const Number r)
  {
    val = (static_cast<unsigned long long>(val) * r.val) % mod;
  }

  unsigned int val;
};
Number fr[maxn + 10], invFr[maxn + 10], invPower[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number modularInv(const Number x) { return quickPow(x, mod - 2); }

static void initPower(const unsigned int n, const long long e, Number dest[])
{
  if (e >= 0) {
    for (unsigned int i = 1; i <= n; ++i)
      dest[i] = quickPow(Number{i}, e);
  }
  else {
    for (unsigned int i = 1; i <= n; ++i)
      dest[i] = quickPow(Number{i}, -e * (mod - 2));
  }
}

template <bool rev>
void inverse(Number dest[], const int n)
{
  if (rev) {
    for (int i = n; i; --i)
      for (int j = i + i; j <= n; j += i)
        dest[i] -= dest[j];
  }
  else {
    for (int i = 1; i <= n; ++i)
      for (int j = i + i; j <= n; j += i)
        dest[j] -= dest[i];
  }
}
static void init(const int n, const int c, const int d)
{
  initPower(n, -d, invPower);
  initPower(n, c - d, fr);
  inverse<false>(fr, n);
  for (int i = 1; i <= n; ++i)
    invFr[i] = modularInv(fr[i]);
}

bool solve(Number b[], const int n)
{
  for (int i = 1; i <= n; ++i)
    b[i] *= invPower[i];
  inverse<false>(b, n);
  for (int i = 1; i <= n; ++i)
    if (fr[i].val)
      b[i] *= invFr[i];
    else if (b[i].val)
      return false;
  inverse<true>(b, n);
  for (int i = 1; i <= n; ++i)
    b[i] *= invPower[i];
  return true;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, c, d, q;
  std::cin >> n >> c >> d >> q;
  init(n, c, d);
  for (int i = q; i; --i) {
    static Number b[maxn + 10];
    for (Number *i = b + 1; i < b + 1 + n; ++i)
      std::cin >> i->val;
    if (solve(b, n)) {
      for (const Number *i = b + 1; i < b + n + 1; ++i)
        std::cout << i->val << " ";
      std::cout.put('\n');
    }
    else
      std::cout << "-1\n";
  }
  return 0;
}