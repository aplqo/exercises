#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1u << 21;

typedef long double Real;
struct Complex {
  Complex() = default;
  Complex(const Real v, const Real m = 0) : mreal(v), imag(m) {}
  inline Complex operator*(const Complex b) const
  {
    return Complex{mreal * b.mreal - imag * b.imag,
                   imag * b.mreal + mreal * b.imag};
  }
  inline void operator*=(const Complex b) { *this = *this * b; }
  inline Complex operator+(const Complex b) const
  {
    return Complex{mreal + b.mreal, imag + b.imag};
  }
  inline Complex operator-(const Complex b) const
  {
    return Complex{mreal - b.mreal, imag - b.imag};
  }
  inline Complex operator/(const Real v) const
  {
    return Complex{mreal / v, imag / v};
  }
  inline Real real() const { return mreal; }

  Real mreal, imag;
};
constexpr Real pi = 3.141592653589793;
unsigned int rev[maxn + 10];

static void initReverse(const unsigned int lgn)
{
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < lgn; ++i)
    rev[1u << i] = 1u << (lgn - 1 - i);
  for (unsigned int i = 3; i < n; ++i) {
    const unsigned int lb = i & -i;
    rev[i] = rev[i ^ lb] | rev[lb];
  }
}
template <class T, class U>
inline void reverseCopy(U dest[], const T src[], const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    dest[rev[i]] = src[i];
}
template <int rat, class T>
void dft(const T a[], Complex res[], const unsigned int n)
{
  reverseCopy(res, a, n);
  for (unsigned int step = 2; step <= n; step <<= 1) {
    const Complex wm{cos(rat * 2 * pi / step), sin(rat * 2 * pi / step)};
    const unsigned int half = step >> 1;
    for (unsigned int blk = 0; blk < n; blk += step) {
      Complex w{1, 0};
      for (unsigned int i = 0; i < half; ++i) {
        const Complex u = res[blk + i], v = w * res[blk + i + half];
        res[blk + i] = u + v;
        res[blk + i + half] = u - v;
        w *= wm;
      }
    }
  }
}
inline unsigned int Log2(const unsigned int x)
{
  for (unsigned int i = 0;; ++i)
    if ((1u << i) >= x) return i;
  return 0;
}
static void fft(Real a[], const unsigned int la, Real b[],
                const unsigned int lb, Real res[])
{
  static Complex va[maxn + 10], vb[maxn * 2 + 10];
  const unsigned int lg = Log2(la + lb + 1), n = 1u << lg;
  initReverse(lg);
  dft<1>(a, va, n);
  dft<1>(b, vb, n);
  for (unsigned int i = 0; i < n; ++i)
    vb[i] *= va[i];
  dft<(-1)>(vb, va, n);
  {
    const Real p = n;
    for (unsigned int i = 0; i < n; ++i)
      res[i] = (va[i] / p).real();
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  static Real ma[maxn + 10], mb[maxn + 10], res[maxn * 2 + 10];
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n + 1, ma);
  copy_n(istream_iterator<unsigned int>(cin), m + 1, mb);
  fft(ma, n, mb, m, res);
  for (unsigned int i = 0; i <= n + m; ++i)
    cout << static_cast<long long>(round(res[i])) << " ";
  cout.put('\n');
  return 0;
}