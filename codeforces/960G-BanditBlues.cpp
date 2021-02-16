#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxl = 18, maxn = 1u << maxl;
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

  unsigned long long val;
};
template <class T>
struct Pointer {
  T* ptr;
  unsigned int size;
};
typedef Pointer<const Number> ConstArrayPtr;
typedef Pointer<Number> ArrayPtr;
Number unit[2][maxl + 10];
unsigned int rev[maxl + 10][maxn + 10], lg[maxn + 10];
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
    inv[i].val = mod - (inv[mod % i] * Number{mod / i}).val;
}
static void initFactor(const unsigned int n)
{
  initModularInv(n);
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
static void initLog(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i)
    std::fill(lg + (1u << (i - 1)) + 1, lg + (1u << i) + 1, i);
}
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
}
static void init(const unsigned int n)
{
  const unsigned int lgn = Log2(n + 1), v = 1u << (lgn + 1);
  initFactor(v);
  initUnit(lgn + 1);
  initRev(lgn + 1);
  initLog(lgn + 1);
}

void reverseCopy(Number dest[], const ConstArrayPtr src, const unsigned int lgn)
{
  const unsigned int n = 1u << lgn;
  const unsigned int* const crev = rev[lgn];
  for (unsigned int i = 0; i < src.size; ++i)
    dest[crev[i]] = src.ptr[i];
  for (unsigned int i = src.size; i < n; ++i)
    dest[crev[i]].val = 0;
}
template <bool typ>
void dft(const ConstArrayPtr a, Number dest[], const unsigned int lgn)
{
  reverseCopy(dest, a, lgn);
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
    const Number iv = inv[n];
    for (Number* i = dest; i < dest + n; ++i)
      *i *= iv;
  }
}
void ntt(const ConstArrayPtr a, const ConstArrayPtr b, const ArrayPtr dest)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10];
  const unsigned int lgn = lg[dest.size];
  dft<false>(a, tmpa, lgn);
  dft<false>(b, tmpb, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < n; ++i)
    tmpa[i] *= tmpb[i];
  dft<true>(ConstArrayPtr{tmpa, n}, dest.ptr, lgn);
}

void shift(const ConstArrayPtr a, const unsigned int c, const ArrayPtr dest)
{
  static Number u[maxn + 10], t[maxn + 10];
  {
    Number acc{1};
    for (unsigned int i = 0; i < a.size; ++i) {
      u[i] = acc * ifac[i];
      t[a.size - 1 - i] = a.ptr[i] * fac[i];
      acc *= Number{c};
    }
  }
  ntt(ConstArrayPtr{u, a.size}, ConstArrayPtr{t, a.size},
      ArrayPtr{dest.ptr, a.size * 2});
  std::reverse(dest.ptr, dest.ptr + a.size);
  for (unsigned int i = 0; i < a.size; ++i)
    dest.ptr[i] *= ifac[i];
}
void multiply(const ArrayPtr a, const Number v)
{
  a.ptr[a.size].val = 0;
  for (unsigned int i = a.size; i; --i)
    a.ptr[i] = a.ptr[i] * v + a.ptr[i - 1];
  a.ptr[0] *= v;
}

void stirling(const unsigned int n, Number dest[])
{
  static Number tmp[maxn + 10];
  const unsigned int lgn = (1u << lg[n]) > n ? lg[n] - 1 : lg[n];
  unsigned int acc = 0;
  dest[1].val = 1;
  if (n & (1u << lgn)) acc = 1;
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int csz = acc << 1;
    shift(ConstArrayPtr{dest, acc + 1}, acc, ArrayPtr{tmp, acc + 1});
    ntt(ConstArrayPtr{tmp, acc + 1}, ConstArrayPtr{dest, acc + 1},
        ArrayPtr{dest, csz + 2});
    acc <<= 1;
    if (n & (1u << (lgn - i))) {
      acc |= 1;
      multiply(ArrayPtr{dest, csz + 1}, Number{csz});
    }
  }
}
Number stirling(const unsigned int n, const unsigned int k)
{
  static Number tmp[maxn + 10];
  if (n == 1) return k == 1 ? Number{1} : Number{0};
  stirling(n, tmp);
  return tmp[k];
}
inline Number C(const unsigned int n, const unsigned int m)
{
  return fac[n] * ifac[m] * ifac[n - m];
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, a, b;
  std::cin >> n >> a >> b;
  if (n == 1) {
    std::cout << (a == 1 && b == 1 ? "1" : "0") << "\n";
    return 0;
  }
  init(std::max(n, a + b));
  std::cout << C(a + b - 2, a - 1) * stirling(n - 1, a + b - 2);
  return 0;
}