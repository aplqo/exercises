#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
const int maxn = 1e5;
constexpr unsigned long long mod = 998244353;

struct Number {
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
unsigned int phi[maxn + 10];
Number fac[maxn + 10], ifac[maxn + 10], inv[maxn + 10];

static void eular(const int n)
{
  static int primes[maxn + 10], *pcur = primes, mindiv[maxn + 10];
  phi[1] = 1;
  for (long long i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      phi[i] = i - 1;
      *(pcur++) = i;
    }
    for (const int* j = primes; j < pcur && *j * i <= n && *j <= mindiv[i];
         ++j) {
      mindiv[*j * i] = *j;
      phi[*j * i] = phi[i] * (*j - 1);
    }
    if (const long long v = mindiv[i] * i; v <= n) {
      mindiv[v] = mindiv[i];
      phi[v] = phi[i] * mindiv[i];
    }
  }
}
static void initModularInv(const int n)
{
  inv[0].val = inv[1].val = 1;
  for (int i = 2; i <= n; ++i)
    inv[i].val = mod - (inv[mod % i] * Number{mod / i}).val;
}
static void initFactor(const int n)
{
  fac[0].val = fac[1].val = ifac[0].val = ifac[1].val = 1;
  for (int i = 2; i <= n; ++i) {
    fac[i] = fac[i - 1] * Number{i};
    ifac[i] = ifac[i - 1] * inv[i];
  }
}
static void init(const int n)
{
  eular(n + 1);
  initModularInv(n + 1);
  initFactor(n + 1);
}
int k, n;

inline Number C(const int n, const int m)
{
  return n >= m ? fac[n] * ifac[m] * ifac[n - m] : Number{0};
}
Number seqInEx(const int black, const int white)
{
  Number ret{};
  for (int i = 0; i * (k + 1) <= black && i <= white + 1; ++i)
    ret += Number{i & 0x01 ? mod - 1 : 1} *
           C(black + white - i * (k + 1), white) * C(white + 1, i);
  return ret;
}
Number countPeriod(const int black, const int white)
{
  if (n == k || (white && black <= k)) return C(black + white, white);
  Number ret{};
  for (int i = 0; i <= black && i <= k; ++i)
    ret += Number{i + 1} * seqInEx(black - i, white - 2);
  return ret;
}
Number count(const int black, const int white)
{
  const int g = std::gcd(black, white);
  Number ret{};
  for (int i = 1; i * i <= g; ++i)
    if (!(g % i)) {
      ret += Number{phi[i]} * countPeriod(black / i, white / i);
      if (const int v = g / i; i != v)
        ret += Number{phi[v]} * countPeriod(black / v, white / v);
    }
  return ret * inv[n];
}

int main()
{
  std::ios::sync_with_stdio(false);
  int m;
  std::cin >> n >> m >> k;
  init(n);
  std::cout << count(m, n - m) << "\n";
  return 0;
}