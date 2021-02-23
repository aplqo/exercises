#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
const int maxln = 19, maxn = 1u << maxln;
constexpr unsigned long long mod = 998244353, primitiveRoot = 3;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned long long v = l.val + r.val;
    return Number{v >= mod ? v - mod : v};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{(l.val * r.val) % mod};
  }
  inline void operator*=(const Number r) { val = (val * r.val) % mod; }
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }

  unsigned long long val;
};

Number unit[2][maxln + 10];
Number ifac[maxn + 10];
Number inv[maxn + 10];
unsigned int rev[maxn + 10];

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

static void initUnit(const int lgn)
{
  for (int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(Number{primitiveRoot}, (mod - 1) >> i);
    unit[1][i] = inverse(unit[0][i]);
  }
}
static void initFactor(const int n)
{
  Number acc{1};
  for (int i = 2; i <= n; ++i)
    acc *= Number{i};
  ifac[n] = inverse(acc);
  for (int i = n - 1; i >= 0; --i)
    ifac[i] = ifac[i + 1] * Number{i + 1};
}
static void initRev(const int lgn)
{
  for (int i = 0; i < lgn; ++i)
    rev[1u << i] = 1u << (lgn - 1 - i);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 3; i < n; ++i) {
    const unsigned int lb = i & -i;
    rev[i] = rev[i ^ lb] | rev[lb];
  }
}
static void initModularInv(const int base, const int range)
{
  static Number prd[maxn * 2 + 10], iprd[maxn * 2 + 10];
  prd[0] = Number{base - range};
  for (int i = 1; i <= range * 2; ++i)
    prd[i] = prd[i - 1] * Number{base + i - range};

  iprd[range * 2] = inverse(prd[range * 2]);
  for (int i = range * 2; i; --i) {
    inv[i] = iprd[i] * prd[i - 1];
    iprd[i - 1] = iprd[i] * Number{base + i - range};
  }
  inv[0] = iprd[0];
}
unsigned int Log2(const int x)
{
  for (int i = 0;; ++i)
    if (x <= (1 << i)) return i;
}
static void init(const int n, const int m)
{
  const int lgn = Log2((n + 1) * 2);
  initUnit(lgn);
  initFactor(n + 1);
  initRev(lgn);
  initModularInv(m, n);
}

template <int iv>
void reverseCopy(Number dest[], const Number src[], const int n)
{
  const int cpy = n >> iv;
  for (int i = 0; i < cpy; ++i)
    dest[rev[i]] = src[i];
}
template <bool typ, int iv>
void dft(const Number a[], Number dest[], const int lgn)
{
  const int n = 1u << lgn;
  reverseCopy<iv>(dest, a, n);
  for (int i = 1; i <= lgn; ++i) {
    const int step = 1u << i, half = 1u << (i - 1);
    const Number wm = unit[typ][i];
    for (Number *blk = dest; blk < dest + n; blk += step) {
      Number w{1};
      for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k) {
        const Number u = *j, v = *k * w;
        *j = u + v;
        *k = u - v;
        w *= wm;
      }
    }
  }
  if constexpr (typ) {
    const Number inv = inverse(Number{n});
    for (Number *i = dest; i < dest + n; ++i)
      *i *= inv;
  }
}
template <int iva = 0, int ivb = 0>
void ntt(const Number a[], const Number b[], Number dest[], const int lgn)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10];
  const int n = 1 << lgn;
  dft<false, iva>(a, tmpa, lgn);
  dft<false, ivb>(b, tmpb, lgn);
  for (int i = 0; i < n; ++i)
    tmpa[i] *= tmpb[i];
  dft<true, 0>(tmpa, dest, lgn);
}

void solve(const int n, const int m, const Number p[], Number ans[])
{
  static Number g[maxn + 10], tmp[maxn * 2 + 10];
  for (int i = 0; i <= n; ++i)
    g[i] = p[i] * ifac[i] * ifac[n - i] * Number{(n - i) & 0x01 ? mod - 1 : 1};
  ntt<1, 0>(g, inv, tmp, Log2((n + 1) * 2));

  Number acc{1};
  for (int i = 1; i <= n; ++i)
    acc *= Number{m - i};
  for (int i = 0; i <= n; ++i) {
    acc *= Number{m + i};
    ans[i] = tmp[i + n] * acc;
    acc *= inv[i];
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  static Number f[maxn + 10], ans[maxn + 10];
  for (Number *i = f; i < f + n + 1; ++i)
    std::cin >> i->val;
  init(n, m);
  solve(n, m, f, ans);
  for (const Number *i = ans; i < ans + n + 1; ++i)
    std::cout << i->val << " ";
  return 0;
}