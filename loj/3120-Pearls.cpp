#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
const unsigned int maxl = 18, maxd = 1u << maxl;
constexpr unsigned long long mod = 998244353, primitiveRoot = 3;

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
template <class T>
struct Pointer {
  T* ptr;
  unsigned int size;
};
typedef Pointer<Number> ArrayPtr;
typedef Pointer<const Number> ConstArrayPtr;
unsigned int rev[maxd + 10];
Number unit[2][maxl + 10], fac[maxd + 10], ifac[maxd + 10];

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
    rev[i] = rev[i ^ lb] | rev[lb];
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
static unsigned int init(const unsigned int d)
{
  const unsigned int lgn = Log2(d + 1);
  initUnit(lgn + 1);
  initRev(lgn + 1);
  initFactor(1u << (lgn + 1));
  return lgn;
}

void reverseCopy(Number dest[], const ConstArrayPtr src, const unsigned int n)
{
  for (unsigned int i = 0; i < src.size; ++i)
    dest[rev[i]] = src.ptr[i];
  for (unsigned int i = src.size; i < n; ++i)
    dest[rev[i]].val = 0;
}
template <bool typ>
void dft(const ConstArrayPtr a, Number dest[], const unsigned int lgn)
{
  const unsigned int n = 1u << lgn;
  reverseCopy(dest, a, n);
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
void ntt(const ConstArrayPtr a, const ConstArrayPtr b, Number dest[],
         const unsigned int lgn)
{
  static Number tmpa[maxd + 10], tmpb[maxd + 10];
  dft<false>(a, tmpa, lgn + 1);
  dft<false>(b, tmpb, lgn + 1);
  const unsigned int n = 1u << (lgn + 1);
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] *= tmpa[i];
  dft<true>(ConstArrayPtr{tmpb, n}, dest, lgn + 1);
}

inline Number C(const unsigned int n, const unsigned int m)
{
  return fac[n] * ifac[m] * ifac[n - m];
}
void buildLeast(const unsigned int n, const unsigned int d,
                const unsigned int lgd, Number dest[])
{
  static Number tmpa[maxd + 10], tmpb[maxd + 10];
  for (unsigned int i = 0; i <= d; ++i) {
    tmpa[i] = ifac[i] * quickPow(Number{d} - Number{2 * i}, n);
    tmpb[i] = ifac[i];
  }
  ntt(ConstArrayPtr{tmpa, d + 1}, ConstArrayPtr{tmpb, d + 1}, dest, lgd);
  const Number inv2 = inverse(Number{2});
  Number acc{1};
  for (unsigned int i = 0; i <= d; ++i) {
    dest[i] *= fac[i] * C(d, i) * acc;
    acc *= inv2;
  }
}
void binomialInversion(const Number src[], const unsigned int d,
                       const unsigned int lgd, Number dest[])
{
  static Number u[maxd + 10], t[maxd + 10];
  for (unsigned int i = 0; i <= d; ++i) {
    t[d - i] = fac[i] * src[i];
    u[i].val = i & 0x01 ? mod - ifac[i].val : ifac[i].val;
  }
  ntt(ConstArrayPtr{u, d + 1}, ConstArrayPtr{t, d + 1}, dest, lgd);
  std::reverse(dest, dest + d + 1);
  for (unsigned int i = 0; i <= d; ++i)
    dest[i] = dest[i] * ifac[i];
}
Number solve(const unsigned int d, const unsigned int n, const unsigned int m,
             const unsigned int lgd)
{
  static Number src[maxd + 10];
  buildLeast(n, d, lgd, src);
  binomialInversion(src, d, lgd, src);
  return std::accumulate(src + d - std::min(d, (n - 2 * m)), src + d + 1,
                         Number{0});
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int d, n, m;
  std::cin >> d >> n >> m;
  std::cout << solve(d, n, m, init(d)) << "\n";
  return 0;
}