#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int maxl = 18, maxn = 1u << maxl;
constexpr unsigned long long mod = 1004535809, primitiveRoot = 3;

class Number {
 public:
#define decl(x)                                                             \
  inline constexpr friend Number operator x(const Number l, const Number r) \
  {                                                                         \
    return Number{(l.val x r.val) % mod};                                   \
  }                                                                         \
  inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
  decl(+);
  decl(*);
#undef decl
  inline constexpr friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline constexpr Number operator-() const
  {
    return Number{val ? mod - val : 0};
  }
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }

  unsigned long long val;
};
unsigned int rev[maxl + 10][maxn + 10], lg[maxn + 10];
Number unit[2][maxl + 10];
Number fac[maxn + 10], ifac[maxn + 10], inv[maxn + 10];

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
static void initLog(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i)
    std::fill(lg + (1u << (i - 1)) + 1, lg + (1u << i) + 1, i);
}
static void initRev(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unsigned int* const crev = rev[i];
    for (unsigned int j = 0; j < i; ++j)
      crev[1u << j] = 1u << (i - 1 - j);
    const unsigned int n = 1u << i;
    for (unsigned int j = 3; j < n; ++j) {
      const unsigned int lb = j & -j;
      crev[j] = crev[j ^ lb] | crev[lb];
    }
  }
}
static void initModularInv(const unsigned int n)
{
  inv[0].val = inv[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    inv[i] = -inv[mod % i] * Number{mod / i};
}
static void initFactor(const unsigned int n)
{
  fac[0].val = ifac[0].val = 1;
  for (unsigned int i = 1; i <= n; ++i) {
    fac[i] = fac[i - 1] * Number{i};
    ifac[i] = ifac[i - 1] * inv[i];
  }
}
static void initUnit(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(Number{primitiveRoot}, (mod - 1) >> i);
    unit[1][i] = inverse(unit[0][i]);
  }
}
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
}
static void init(const unsigned int n)
{
  const unsigned int lgn = Log2(n) + 1, v = 1u << lgn;
  initRev(lgn);
  initLog(lgn);
  initUnit(lgn);
  initModularInv(v);
  initFactor(v);
}

template <unsigned int inv>
void reverseCopy(Number dest[], const Number src[], const unsigned int lgn)
{
  const unsigned int cpy = 1u << (lgn - inv), n = 1u << lgn;
  const unsigned int* const crev = rev[lgn];
  for (unsigned int i = 0; i < cpy; ++i)
    dest[crev[i]] = src[i];
  for (unsigned int i = cpy; i < n; ++i)
    dest[crev[i]].val = 0;
}
template <bool typ, unsigned int inv>
void dft(const Number a[], Number dest[], const unsigned int lgn)
{
  reverseCopy<inv>(dest, a, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 1; i <= lgn; ++i) {
    const Number wm{unit[typ][i]};
    const unsigned int step = 1u << i, half = 1u << (i - 1);
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
    const Number iv = ::inv[n];
    for (Number* i = dest; i < dest + n; ++i)
      *i *= iv;
  }
}

template <unsigned int ia, unsigned int ib, class Func>
void convolution(const Number a[], const Number b[], Number dest[],
                 const unsigned int lgn, Func pred)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10];
  dft<false, ia + 1>(a, tmpa, lgn + 1);
  dft<false, ib + 1>(b, tmpb, lgn + 1);
  const unsigned int n = 1u << (lgn + 1);
  for (unsigned int i = 0; i < n; ++i)
    tmpa[i] = pred(tmpa[i], tmpb[i]);
  dft<true, 0>(tmpa, dest, lgn + 1);
}
template <unsigned int ia, unsigned int ib>
void ntt(const Number a[], const Number b[], Number dest[],
         const unsigned int lgn)
{
  convolution<ia, ib>(a, b, dest, lgn, std::multiplies<Number>());
}

void inverse(const Number a[], Number dest[], const unsigned int lgn)
{
  dest[0] = inverse(a[0]);
  for (unsigned int i = 1; i <= lgn; ++i)
    convolution<0, 1>(a, dest, dest, i, [](Number a, Number b) {
      return b * (Number{2} - a * b);
    });
}
void derivative(const Number a[], Number dest[], const unsigned int n)
{
  for (unsigned int i = 1; i < n; ++i)
    dest[i - 1] = a[i] * Number{i};
  dest[n - 1].val = 0;
}
void integral(const Number a[], Number dest[], const unsigned int n)
{
  dest[0].val = 0;
  for (unsigned int i = 0; i < n; ++i)
    dest[i + 1] = a[i] * inv[i + 1];
}
void ln(const Number a[], Number dest[], const unsigned int lgn)
{
  static Number tp[maxn + 10], iv[maxn + 10];
  const unsigned int n = 1u << lgn;
  derivative(a, tp, n);
  inverse(a, iv, lgn);
  ntt<0, 0>(tp, iv, iv, lgn);
  integral(iv, dest, n);
}

Number solve(const unsigned int lgn, const unsigned int n)
{
  static Number a[maxn + 10], dest[maxn + 10];
  a[0].val = a[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    a[i] =
        quickPow(Number{2}, static_cast<unsigned long long>(i) * (i - 1) >> 1) *
        ifac[i];
  ln(a, dest, lgn);
  return dest[n] * fac[n];
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n;
  std::cin >> n;
  init(n + 1);
  std::cout << solve(lg[n + 1], n) << "\n";
  return 0;
}