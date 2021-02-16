#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxln = 18, maxn = 1u << maxln;
constexpr unsigned long long mod = 998244353, primeRoot = 3;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline void operator x##=(const Number r)                       \
  {                                                               \
    val = ((val + mod) x r.val) % mod;                            \
  }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline friend ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }
  inline friend istream& operator>>(istream& is, Number& v)
  {
    return is >> v.val;
  }
  inline Number operator-() const
  {
    Number ret;
    ret.val = mod - val;
    return ret;
  }

 private:
  unsigned long long val = 0;
};
struct ConstArrayPtr {
  const Number* ptr;
  unsigned int size;
};
struct ArrayPtr {
  Number* ptr;
  unsigned int size;
  inline operator ConstArrayPtr() const { return ConstArrayPtr{ptr, size}; }
};
Number unit[2][maxln + 10];
unsigned int rev[maxln + 10][maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
static void initUnit(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
    unit[1][i] = quickPow(unit[0][i], mod - 2);
  }
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
static void init(const unsigned int lgn)
{
  initUnit(lgn + 1);
  initRev(lgn + 1);
}

unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
  return 0;
}
void reverseCopy(Number dest[], const ConstArrayPtr src, const unsigned int lgn)
{
  static const Number zero;
  const unsigned int n = 1u << lgn;
  const unsigned int* const crev = rev[lgn];
  for (unsigned int i = 0; i < src.size; ++i)
    dest[crev[i]] = src.ptr[i];
  for (unsigned int i = src.size; i < n; ++i)
    dest[crev[i]] = zero;
}
template <unsigned int typ>
void dft(const ConstArrayPtr src, Number dest[], const unsigned int lgn)
{
  reverseCopy(dest, src, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int step = 1u << i, half = 1u << (i - 1);
    const Number wm = unit[typ][i];
    for (unsigned int blk = 0; blk < n; blk += step) {
      Number w = 1;
      for (unsigned int j = 0; j < half; ++j) {
        const Number u = dest[blk + j], v = w * dest[blk + j + half];
        dest[blk + j] = u + v;
        dest[blk + j + half] = u - v;
        w *= wm;
      }
    }
  }
  if constexpr (typ) {
    const Number iv = quickPow(n, mod - 2);
    for (Number* i = dest; i < dest + n; ++i)
      *i *= iv;
  }
}

template <bool op1, bool op2, bool op3, class Func>
void convolution(const ConstArrayPtr& a, const ConstArrayPtr& b, Number dest[],
                 unsigned int lgn, Func pred)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10];
  const unsigned int n = 1u << lgn;
  dft<op1>(a, tmpa, lgn);
  dft<op2>(b, tmpb, lgn);
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] = pred(tmpa[i], tmpb[i]);
  dft<op3>(ConstArrayPtr{tmpb, n}, dest, lgn);
}

inline void inverse(const ConstArrayPtr a, ArrayPtr& dest)
{
  static const auto pred = [](Number a, Number b) { return b * (2 - a * b); };
  const unsigned int lgn = Log2(a.size);
  dest.ptr[0] = quickPow(a.ptr[0], mod - 2);
  for (unsigned int i = 1; i < lgn; ++i)
    convolution<false, false, true>(ConstArrayPtr{a.ptr, 1u << i},
                                    ConstArrayPtr{dest.ptr, 1u << (i - 1)},
                                    dest.ptr, i + 1, pred);
  convolution<false, false, true>(a, ConstArrayPtr{dest.ptr, 1u << (lgn - 1)},
                                  dest.ptr, lgn + 1, pred);
  dest.size = a.size;
}
template <bool op1, bool op2, bool op3>
inline void ntt(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr& dest)
{
  dest.size = a.size + b.size - 1;
  convolution<op1, op2, op3>(a, b, dest.ptr, Log2(dest.size),
                             multiplies<Number>());
}

Number mem[(maxn + 10) * maxln * 8], *cur = mem;
ConstArrayPtr g[maxn * 4 + 10], t[maxn * 4 + 10];

template <bool op1, bool op2, bool op3>
inline ConstArrayPtr multiply(const ConstArrayPtr& a, const ConstArrayPtr& b)
{
  ArrayPtr tp{cur, 0};
  ntt<op1, op2, op3>(a, b, tp);
  return ConstArrayPtr{tp.ptr, tp.size};
}
inline ConstArrayPtr transMultiply(const Number vx[], const ConstArrayPtr& y,
                                   const unsigned int lgn)
{
  static Number tmpb[maxn + 10];
  dft<false>(y, tmpb, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] *= vx[i];
  ArrayPtr ret{cur, n};
  dft<false>(ConstArrayPtr{tmpb, n}, cur, lgn);
  return ret;
}

void productX(const unsigned int cid, const unsigned int l,
              const unsigned int lg, const Number x[])
{
  if (!lg) {
    cur[0] = 1;
    cur[1] = -x[l];
    new (&g[cid]) ConstArrayPtr{cur, 2};
    cur += 2;
    return;
  }
  const unsigned int left = cid << 1, right = left + 1;
  productX(left, l, lg - 1, x);
  productX(right, l + (1u << (lg - 1)), lg - 1, x);
  g[cid] = multiply<false, false, true>(g[left], g[right]);
  cur += g[cid].size;
}
void evalPolynomial(const unsigned int cid, const unsigned int l,
                    const unsigned int lg, Number dest[])
{
  if (!lg) {
    dest[l] = t[cid].ptr[0];
    return;
  }
  const unsigned int left = cid << 1, right = left + 1, half = 1u << (lg - 1);
  {
    static Number tmpv[maxn + 10];
    const unsigned int lgn = Log2(t[cid].size + g[right].size - 1);
    dft<true>(t[cid], tmpv, lgn);
    t[left] = transMultiply(tmpv, g[right], lgn);
    t[left].size = half;
    cur += half;
    t[right] = transMultiply(tmpv, g[left], lgn);
    t[right].size = half;
    cur += half;
  }
  evalPolynomial(left, l, lg - 1, dest);
  evalPolynomial(right, l + half, lg - 1, dest);
}

void eval(const Number pol[], const Number pos[], Number dest[],
          const unsigned int lgn)
{
  productX(1, 0, lgn, pos);
  {
    static Number tmp[maxn + 10];
    const unsigned int n = 1u << lgn;
    ArrayPtr tp{cur, 0}, iv{tmp};
    inverse(g[1], iv);
    ntt<true, false, false>(ConstArrayPtr{pol, 1u << lgn}, iv, tp);
    tp.size = n;
    cur += tp.size;
    t[1] = tp;
  }
  evalPolynomial(1, 0, lgn, dest);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  static Number a[maxn + 10], pos[maxn + 10], dest[maxn + 10];
  cin >> n >> m;
  copy_n(istream_iterator<Number>(cin), n + 1, a);
  copy_n(istream_iterator<Number>(cin), m, pos);
  const unsigned int l = Log2(max(n + 1, m));
  init(l + 1);
  eval(a, pos, dest, l);
  copy(dest, dest + m, ostream_iterator<Number>(cout, "\n"));
  return 0;
}