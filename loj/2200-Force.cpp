#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <cmath>
#include <complex>
#include <iostream>
const unsigned int maxl = 18, maxn = 1u << maxl;
typedef double Real;
typedef std::complex<Real> Complex;
constexpr Real pi = 3.141592653589793;

unsigned int rev[maxn + 10];

static void initRev(const unsigned int lgn)
{
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < lgn; ++i)
    rev[1u << i] = 1u << (lgn - 1 - i);
  for (unsigned int i = 3; i < n; ++i) {
    const unsigned int lb = i & -i;
    rev[i] = rev[i ^ lb] | rev[lb];
  }
}
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
  return 0;
}
template <unsigned int inv, class T, class U>
void reverseCopy(T dest[], const U src[], const unsigned int n)
{
  const unsigned int cpy = n >> inv;
  for (unsigned int i = 0; i < cpy; ++i)
    dest[rev[i]] = src[i];
  for (unsigned int i = cpy; i < n; ++i)
    dest[rev[i]] = T{};
}
template <unsigned int invalid, int r, class T>
void dft(const T src[], Complex dest[], const unsigned int n)
{
  reverseCopy<invalid>(dest, src, n);
  for (unsigned int step = 2; step <= n; step <<= 1) {
    const Complex wm{std::cos(r * 2 * pi / step), std::sin(r * 2 * pi / step)};
    const unsigned int half = step >> 1;
    for (Complex* blk = dest; blk < dest + n; blk += step) {
      Complex w{1, 0};
      for (Complex *i = blk, *j = blk + half; i < blk + half; ++i, ++j) {
        const Complex u = *i, v = *j * w;
        *i = u + v;
        *j = u - v;
        w *= wm;
      }
    }
  }
}
void fft(const Real a[], const Real b[], Real dest[], unsigned int n)
{
  static Complex tmpa[maxn + 10], tmpb[maxn + 10];
  n <<= 1;
  dft<1, 1>(a, tmpa, n);
  dft<1, 1>(b, tmpb, n);
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] *= tmpa[i];
  dft<0, -1>(tmpb, tmpa, n);
  for (unsigned int i = 0; i < n; ++i)
    dest[i] = tmpa[i].real() / n;
}
Real squ[maxn + 10], a[maxn + 10], e[maxn + 10];

void solve(const unsigned int n)
{
  static Real tmpa[maxn + 10], tmpRev[maxn + 10];
  const unsigned int lgn = Log2(n + 1);
  initRev(lgn + 1);
  for (unsigned int i = 1; i <= n; ++i)
    squ[i] = 1.0 / i / i;
  fft(squ, a, tmpa, 1u << lgn);
  std::reverse(a, a + n + 1);
  fft(squ, a, tmpRev, 1u << lgn);
  for (unsigned int i = 1; i <= n; ++i)
    e[i] = tmpa[i] - tmpRev[n - i];
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n;
  std::cin >> n;
  std::copy_n(std::istream_iterator<Real>(std::cin), n, a + 1);
  solve(n);
  std::cout.setf(std::ios::fixed);
  std::copy(e + 1, e + 1 + n, std::ostream_iterator<Real>(std::cout, "\n"));
  return 0;
}