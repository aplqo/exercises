#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxlk = 16, maxk = 1u << maxlk;
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
    static constexpr long long llMod = mod;
    int tp;
    is >> tp;
    v.val = (tp % llMod + llMod) % llMod;
    return is;
  }
  inline Number operator-() const
  {
    Number ret;
    ret.val = val ? mod - val : 0;
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
Number unit[2][maxlk + 10];
unsigned int rev[maxlk + 10][maxk + 10], lg[maxk + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if ((1u << i) >= n) return i;
  return 0;
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
static void initLog(const unsigned int lgn)
{
  unsigned int cur = 1;
  for (unsigned int i = 0; i <= lgn; ++i) {
    const unsigned int nxt = 1u << i;
    fill(lg + cur, lg + nxt + 1, i);
    cur = nxt + 1;
  }
}
inline void reverseCopy(Number dest[], const ConstArrayPtr src,
                        const unsigned int lgn)
{
  static const Number zero;
  const unsigned int* const crev = rev[lgn];
  for (unsigned int i = 0; i < src.size; ++i)
    dest[crev[i]] = src.ptr[i];
  const unsigned int n = 1u << lgn;
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
    for (Number* blk = dest; blk < dest + n; blk += step) {
      Number w = 1;
      for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k) {
        const Number u = *j, v = w * (*k);
        *j = u + v;
        *k = u - v;
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
template <class Func>
inline void convolution(const ConstArrayPtr& a, const ConstArrayPtr& b,
                        ArrayPtr dest, Func pred)
{
  static Number tmpa[maxk + 10], tmpb[maxk + 10];
  const unsigned int lgn = lg[dest.size];
  dest.size = 1u << lgn;
  dft<0>(a, tmpa, lgn);
  dft<0>(b, tmpb, lgn);
  for (unsigned int i = 0; i < dest.size; ++i)
    tmpb[i] = pred(tmpa[i], tmpb[i]);
  dft<1>(ConstArrayPtr{tmpb, dest.size}, dest.ptr, lgn);
}

void inverse(const ConstArrayPtr src, ArrayPtr dest)
{
  static const auto pred = [](Number a, Number b) { return b * (2 - a * b); };
  dest.ptr[0] = quickPow(src.ptr[0], mod - 2);
  const unsigned int lgn = lg[dest.size];
  Number* const pDest = dest.ptr;
  unsigned int i = 1;
  for (; (1u << i) <= src.size; ++i)
    convolution(ConstArrayPtr{src.ptr, 1u << i},
                ConstArrayPtr{pDest, 1u << (i - 1)},
                ArrayPtr{pDest, 1u << (i + 1)}, pred);
  for (; i <= lgn; ++i)
    convolution(src, ConstArrayPtr{pDest, 1u << (i - 1)},
                ArrayPtr{pDest, 1u << (i + 1)}, pred);
}
void ntt(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr& dest)
{
  dest.size = a.size + b.size - 1;
  convolution(a, b, dest, multiplies<Number>());
}
void modular(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr& mod)
{
  static Number rev[maxk + 10], tmp[maxk + 10], tmpdiv[maxk + 10];
  if (a.size < b.size) {
    mod.size = a.size;
    if (a.ptr == mod.ptr) return;
    copy(a.ptr, a.ptr + a.size, mod.ptr);
    return;
  }
  const unsigned int dsize = a.size + 1 - b.size;
  ArrayPtr iv{tmp, dsize + 1}, div{tmpdiv};
  {
    reverse_copy(b.ptr, b.ptr + b.size, rev);
    inverse(ConstArrayPtr{rev, b.size}, iv);
  }
  reverse_copy(a.ptr, a.ptr + a.size, rev);
  ntt(ConstArrayPtr{rev, dsize + 1}, iv, div);
  div.size = dsize;
  reverse(div.ptr, div.ptr + dsize);

  {
    ArrayPtr pTmp{tmp};
    ntt(b, div, pTmp);
    mod.size = b.size - 1;
    for (unsigned int i = 0; i < mod.size; ++i)
      mod.ptr[i] = a.ptr[i] - tmp[i];
  }
}

/// @param pol x^k - f_i x^i
Number recursion(const ConstArrayPtr pol, const Number iv[],
                 unsigned long long n)
{
  static Number acc[maxk + 10], ans[maxk + 10];
  if (n < pol.size - 1) return iv[n];
  acc[0] = 0;
  acc[1] = ans[0] = 1;
  ArrayPtr pAcc{acc, 2}, pAns{ans, 1};
  for (; n; n >>= 1) {
    if (n & 0x01) {
      ntt(pAcc, pAns, pAns);
      modular(pAns, pol, pAns);
    }
    ntt(pAcc, pAcc, pAcc);
    modular(pAcc, pol, pAcc);
  }
  Number ret = 0;
  for (unsigned int i = 0; i < pAns.size; ++i)
    ret += pAns.ptr[i] * iv[i];
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long n;
  unsigned int k;
  cin >> n >> k;
  Number f[maxk + 10], a[maxk + 10];
  for (Number* i = f + k - 1; i >= f; --i) {
    cin >> *i;
    *i = -*i;
  }
  copy_n(istream_iterator<Number>(cin), k, a);
  f[k] = 1;
  const unsigned int lg = Log2(k + 1) + 1;
  initLog(lg);
  initUnit(lg);
  initRev(lg);
  cout << recursion(ConstArrayPtr{f, k + 1}, a, n) << "\n";
  return 0;
}