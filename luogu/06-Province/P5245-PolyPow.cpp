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
  inline void operator x##=(const Number r) { *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline friend istream& operator>>(istream& is, Number& v)
  {
    return is >> v.val;
  }
  inline friend ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
Number unit[2][maxln + 10], inv[maxn + 10];
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
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
  return 0;
}
static void initRev(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unsigned int* const crev = rev[i];
    for (unsigned int j = 0; j < lgn; ++j)
      crev[1u << j] = 1u << (i - 1 - j);
    const unsigned int n = 1u << i;
    for (unsigned int j = 3; j < n; ++j) {
      const unsigned int lb = j & -j;
      crev[j] = crev[j ^ lb] | crev[lb];
    }
  }
}
static void initUnit(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
    unit[1][i] = quickPow(unit[0][i], mod - 2);
  }
}
static void initModularInv(const unsigned int n)
{
  static Number prd[maxn + 10], iprd[maxn + 10];
  prd[0] = prd[1] = iprd[0] = 1;
  for (unsigned int i = 2; i <= n; ++i)
    prd[i] = prd[i - 1] * i;
  iprd[n] = quickPow(prd[n], mod - 2);
  for (unsigned int i = n; i; --i) {
    inv[i] = iprd[i] * prd[i - 1];
    iprd[i - 1] = iprd[i] * i;
  }
  inv[0] = 1;
}
template <unsigned int invalid>
void reverseCopy(Number dest[], const Number src[], const unsigned int lgn)
{
  static const Number zero;
  const unsigned int* const crev = rev[lgn];
  const unsigned int cpy = 1u << (lgn - invalid), n = 1u << lgn;
  for (unsigned int i = 0; i < cpy; ++i)
    dest[crev[i]] = src[i];
  for (unsigned int i = cpy; i < n; ++i)
    dest[crev[i]] = zero;
}
template <unsigned int typ, unsigned int invalid>
void dft(const Number a[], Number dest[], const unsigned int lgn)
{
  reverseCopy<invalid>(dest, a, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int step = 1u << i, half = 1u << (i - 1);
    const Number wm = unit[typ][i];
    for (unsigned int blk = 0; blk < n; blk += step) {
      Number w = 1;
      for (unsigned int i = 0; i < half; ++i) {
        const Number u = dest[blk + i], v = w * dest[blk + i + half];
        dest[blk + i] = u + v;
        dest[blk + i + half] = u - v;
        w *= wm;
      }
    }
  }
  if constexpr (typ) {
    const Number iv = quickPow(n, mod - 2);
    for (unsigned int i = 0; i < n; ++i)
      dest[i] *= iv;
  }
}
template <unsigned int ia, unsigned int ib, class Func>
void convolution(const Number a[], const Number b[], Number dest[],
                 const unsigned int lgn, Func pred)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10];
  dft<0, ia + 1>(a, tmpa, lgn + 1);
  dft<0, ib + 1>(b, tmpb, lgn + 1);
  const unsigned int n = 1u << (lgn + 1);
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] = pred(tmpa[i], tmpb[i]);
  dft<1, 0>(tmpb, dest, lgn + 1);
}

template <unsigned int ia, unsigned int ib>
void ntt(const Number a[], const Number b[], Number dest[],
         const unsigned int lgn)
{
  convolution<ia, ib>(a, b, dest, lgn, multiplies<Number>());
}
template <unsigned int ia>
void inverse(const Number a[], Number dest[], const unsigned int lgn)
{
  static const auto pred = [](Number a, Number b) { return b * (2 - a * b); };
  dest[0] = quickPow(a[0], mod - 2);
  for (unsigned int i = 1; i < lgn; ++i)
    convolution<0, 1>(a, dest, dest, i, pred);
  convolution<ia, 1>(a, dest, dest, lgn, pred);
}
inline void derivative(const Number a[], Number dest[], const unsigned int n)
{
  for (unsigned int i = 0; i + 1 < n; ++i)
    dest[i] = a[i + 1] * (i + 1);
  dest[n - 1] = 0;
}
inline void integral(const Number a[], Number dest[], const unsigned int n)
{
  for (unsigned int i = 1; i < n; ++i)
    dest[i] = a[i - 1] * inv[i];
  dest[0] = 0;
}

template <unsigned int ia>
void ln(const Number a[], Number dest[], const unsigned int lgn)
{
  static Number iv[maxn + 10], tp[maxn + 10];
  const unsigned int n = 1u << lgn;
  inverse<ia>(a, iv, lgn);
  derivative(a, tp, n >> ia);
  ntt<0, ia>(iv, tp, iv, lgn);
  integral(iv, dest, n);
}
void exp(const Number a[], Number dest[], const unsigned int lgn)
{
  static Number tmp[maxn + 10];
  dest[0] = 1;
  for (unsigned int i = 1; i <= lgn; ++i) {
    ln<1>(dest, tmp, i);
    const unsigned int n = 1u << i;
    for (unsigned int i = 0; i < n; ++i)
      tmp[i] = a[i] - tmp[i];
    tmp[0] += 1;
    ntt<0, 1>(tmp, dest, dest, i);
  }
}

void pow(const Number a[], Number dest[], const unsigned int lgn,
         const Number k)
{
  static Number tmp[maxn + 10];
  ln<0>(a, tmp, lgn);
  {
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 0; i < n; ++i)
      tmp[i] *= k;
  }
  exp(tmp, dest, lgn);
}
Number readNumber()
{
  string str;
  cin >> str;
  Number ret;
  for (auto i = str.cbegin(); i != str.cend(); ++i)
    ret = ret * 10 + (*i - '0');
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  const Number k = readNumber();
  static Number a[maxn + 10], b[maxn + 10];
  copy_n(istream_iterator<Number>(cin), n, a);
  const unsigned int l = Log2(n);
  initUnit(l + 1);
  initRev(l + 1);
  initModularInv(1u << l);
  pow(a, b, l, k);
  copy(b, b + n, ostream_iterator<Number>(cout, " "));
  cout.put('\n');
  return 0;
}