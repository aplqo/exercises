#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>
typedef double Real;
typedef std::complex<Real> Complex;
const Real pi(3.141592653589793L);
const int maxlv = 17, maxv = 1u << maxlv, maxn = 1e5;

template <class T>
struct Pointer {
  T* ptr;
  int size;
};
Complex unit[2][maxlv + 10];
unsigned int rev[maxlv + 10][maxv + 10];
int lg[maxv + 10];

static void initUnit(const int lgv)
{
  const Real pi2 = 2 * pi;
  for (int i = 1; i <= lgv; ++i) {
    const int step = 1 << i;
    unit[0][i] = Complex{cos(pi2 / step), sin(pi2 / step)};
    unit[1][i] = Complex{cos(-pi2 / step), sin(-pi2 / step)};
  }
}
static void initLog(const int lgv)
{
  for (int i = 1; i <= lgv; ++i)
    std::fill(lg + (1u << (i - 1)) + 1, lg + (1u << i) + 1, i);
}
static void initRev(const int lgv)
{
  for (int i = 1; i <= lgv; ++i) {
    unsigned int* const crev = rev[i];
    for (int j = 0; j < i; ++j)
      crev[1u << j] = 1u << (i - 1 - j);
    const unsigned int n = 1u << i;
    for (unsigned int j = 3; j < n; ++j) {
      const unsigned int lb = j & -j;
      crev[j] = crev[j ^ lb] | crev[lb];
    }
  }
}
int Log2(const int x)
{
  for (int i = 0;; ++i)
    if (x <= (1 << i)) return i;
}
static void init(const int v)
{
  const int lgv = Log2(v * 2) + 1;
  initLog(lgv);
  initRev(lgv);
  initUnit(lgv);
}

template <class U, class T>
void reverseCopy(U dest[], Pointer<const T> src, const int lgn)
{
  const unsigned int* const crev = rev[lgn];
  const int n = 1u << lgn;
  for (int i = 0; i < src.size; ++i)
    dest[crev[i]] = src.ptr[i];
  for (int i = src.size; i < n; ++i)
    dest[crev[i]] = Complex{};
}
template <bool inv, class T>
void dft(const Pointer<const T> src, Complex dest[], const int lgn)
{
  reverseCopy(dest, src, lgn);
  const int n = 1u << lgn;
  for (int i = 1; i <= lgn; ++i) {
    const Complex wm = unit[inv][i];
    const int half = 1u << (i - 1), step = 1u << i;
    for (Complex* blk = dest; blk < dest + n; blk += step) {
      Complex w{1};
      for (Complex *j = blk, *k = blk + half; j < blk + half; ++j, ++k) {
        const Complex u = *j, v = *k * w;
        *j = u + v;
        *k = u - v;
        w *= wm;
      }
    }
  }
}
void fft(const Pointer<const int> a, const Pointer<const int> b,
         long long dest[])
{
  static Complex tmpa[maxv + 10], tmpb[maxv + 10];
  const int lgn = lg[a.size + b.size - 1], n = 1u << lgn;
  dft<false>(a, tmpa, lgn);
  dft<false>(b, tmpb, lgn);
  for (int i = 0; i < n; ++i)
    tmpa[i] *= tmpb[i];
  dft<true>(Pointer<const Complex>{tmpa, n}, tmpb, lgn);
  for (int i = 0; i < n; ++i)
    dest[i] = round(tmpb[i].real() / n);
}

int a[maxn + 10];
long long count(const int n, const int ma)
{
  static long long cnt[maxv + 10];
  static int prev[maxv + 10], next[maxv + 10];

  if (n < 3) return 0;

  const int block = sqrt(n * log2(n));
  long long ret = 0;
  for (const int* i = a; i < a + n; ++i)
    ++next[*i];

  for (const int* i = a; i < a + n; i += block) {
    static int internal[maxv + 10];

    const int* end = std::min<const int*>(a + n, i + block);
    for (const int* j = i; j < end; ++j) {
      --next[*j];
      ++internal[*j];
    }
    fft(Pointer<const int>{prev, ma + 1}, Pointer<const int>{next, ma + 1},
        cnt);

    for (const int* j = i; j < end; ++j) {
      --internal[*j];
      const int v = *j * 2;
      ret += cnt[v];
      for (const int* k = i; k < j; ++k)
        if (v >= *k) ret += internal[v - *k] + next[v - *k];
      for (const int* k = j + 1; k < end; ++k)
        if (v >= *k) ret += prev[v - *k];
    }

    for (const int* j = i; j < end; ++j) {
      ++prev[*j];
      internal[*j] = 0;
    }
  }
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, ma = 0;
  std::cin >> n;
  for (int* i = a; i < a + n; ++i) {
    std::cin >> *i;
    ma = std::max(ma, *i);
  }
  init(ma + 1);
  std::cout << count(n, ma) << "\n";
  return 0;
}