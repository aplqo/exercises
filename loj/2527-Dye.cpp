#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxl = 18, maxm = 1u << maxl, maxn = 1e7;
constexpr unsigned long long mod = 1004535809, primitiveRoot = 3;

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
  inline Number operator-() const { return Number{val ? mod - val : 0}; }
  unsigned long long val;
};
Number unit[2][maxl + 10];
unsigned int rev[maxm + 10], w[maxm + 10];
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
inline Number inverse(const Number a) { return quickPow(a, mod - 2); }
static void initUnit(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(Number{primitiveRoot}, (mod - 1) >> i);
    unit[1][i] = inverse(unit[0][i]);
  }
}
static void initRev(const unsigned int lgn)
{
  for (unsigned int i = 0; i < lgn; ++i)
    rev[1u << i] = 1u << (lgn - 1 - i);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 3; i < n; ++i) {
    const unsigned int lb = i & -i;
    rev[i] = rev[lb] | rev[i ^ lb];
  }
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
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
}
static unsigned int init(const unsigned int n, const unsigned int m)
{
  const unsigned int lgm = Log2(m + 1);
  initRev(lgm + 1);
  initUnit(lgm + 1);
  initFactor(std::max(n, m));
  return lgm;
}

template <unsigned int inv>
void reverseCopy(Number dest[], const Number src[], const unsigned int lgn)
{
  const unsigned int cpy = 1u << (lgn - inv), n = 1u << lgn;
  for (unsigned int i = 0; i < cpy; ++i)
    dest[rev[i]] = src[i];
  for (unsigned int i = cpy; i < n; ++i)
    dest[rev[i]].val = 0;
}
template <bool typ, unsigned int inv>
void dft(const Number a[], Number dest[], const unsigned int lgn)
{
  reverseCopy<inv>(dest, a, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int step = 1u << i, half = 1u << (i - 1);
    const Number wm = unit[typ][i];
    for (Number* blk = dest; blk < dest + n; blk += step) {
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
    const Number iv = inverse(Number{n});
    for (Number* i = dest; i < dest + n; ++i)
      *i *= iv;
  }
}
void ntt(const Number a[], const Number b[], Number dest[],
         const unsigned int lgn)
{
  static Number tmpa[maxm + 10], tmpb[maxm + 10];
  dft<false, 1>(a, tmpa, lgn + 1);
  dft<false, 1>(b, tmpb, lgn + 1);
  const unsigned int n = 1u << (lgn + 1);
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] *= tmpa[i];
  dft<true, 0>(tmpb, dest, lgn + 1);
}

inline Number C(const unsigned int n, const unsigned int m)
{
  return fac[n] * ifac[n - m] * ifac[m];
}
Number solve(const unsigned int n, const unsigned int m, const unsigned int s,
             const unsigned int lgm)
{
  static Number h[maxm + 10], t[maxm + 10];
  const unsigned int minm = std::min(m, n / s);
  {
    Number acc{1};
    for (unsigned int i = 0, cnt = 0; i <= m && cnt <= n; ++i, cnt += s) {
      h[i] = Number{i & 0x01 ? mod - ifac[i].val : ifac[i].val};
      t[minm - i] = fac[i] * C(m, i) * fac[n] * ifac[n - cnt] * acc *
                    quickPow(Number{m - i}, n - cnt);
      acc *= ifac[s];
    }
  }
  ntt(h, t, h, lgm);
  std::reverse(h, h + minm + 1);
  Number ret{0};
  for (unsigned int i = 0; i <= minm; ++i)
    ret += Number{w[i]} * h[i] * ifac[i];
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, m, s;
  std::cin >> n >> m >> s;
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), m + 1, w);
  std::cout << solve(n, m, s, init(n, m)) << "\n";
  return 0;
}