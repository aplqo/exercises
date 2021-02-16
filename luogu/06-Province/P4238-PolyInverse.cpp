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
  inline Number operator-() const { return Number{mod - val}; }
  inline friend ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }
  inline friend istream& operator>>(istream& is, Number& v)
  {
    return is >> v.val;
  }

 private:
  unsigned long long val = 0;
};
Number unit[2][maxln + 1];
unsigned int rev[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0, acc = 1;; acc <<= 1, ++i)
    if (acc >= n) return i;
  return 0;
}
static void initUnit(const unsigned int lgn)
{
  for (unsigned int i = 0; i <= lgn; ++i) {
    unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
    unit[1][i] = quickPow(unit[0][i], mod - 2);
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
template <unsigned int invalid>
inline void reverseCopy(Number dest[], const Number src[], const unsigned int n)
{
  const static Number zero;
  const unsigned int half = n >> invalid;
  for (unsigned int i = 0; i < half; ++i)
    dest[rev[i]] = src[i];
  for (unsigned int i = half; i < n; ++i)
    dest[rev[i]] = zero;
}
template <unsigned int typ, unsigned int invalid>
void dft(const Number src[], Number dest[], const unsigned int lgn)
{
  const unsigned int n = 1u << lgn;
  reverseCopy<invalid>(dest, src, n);
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int half = 1u << (i - 1), step = 1u << i;
    const Number wm = unit[typ][i];
    for (unsigned int blk = 0; blk < n; blk += step) {
      Number w = 1;
      for (unsigned int i = 0; i < half; ++i) {
        const Number u = dest[blk + i], v = w * dest[blk + half + i];
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

void inverse(const Number a[], Number out[], const unsigned int lgn)
{
  static Number tmpa[maxn + 10], tmpb[maxn + 10];
  out[0] = quickPow(a[0], mod - 2);
  for (unsigned int i = 1; i <= lgn; ++i) {
    initRev(i + 1);
    dft<0, 1>(a, tmpa, i + 1);
    dft<0, 2>(out, tmpb, i + 1);
    {
      const unsigned int sz = 1u << (i + 1);
      for (unsigned int j = 0; j < sz; ++j)
        tmpb[j] = tmpb[j] * (2 - tmpa[j] * tmpb[j]);
    }
    dft<1, 0>(tmpb, out, i + 1);
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  static Number a[maxn + 10], res[maxn + 10];
  copy_n(istream_iterator<Number>(cin), n, a);
  const unsigned int lgn = Log2(n);
  initUnit(lgn + 1);
  inverse(a, res, lgn);
  copy(res, res + n, ostream_iterator<Number>(cout, " "));
  cout.put('\n');
  return 0;
}