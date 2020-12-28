#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxln = 19, maxn = 1u << maxln;
typedef long double Real;
typedef complex<Real> Complex;
constexpr Real pi = 3.141592653589793;

long long a[maxn + 10], b[maxn + 10];
unsigned int rev[maxn + 10];

unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 0;; ++i)
        if (n <= (1u << i))
            return i;
    return 0;
}
static void initRev(const unsigned int lgn)
{
    for (unsigned int i = 0; i < lgn; ++i)
        rev[1u << i] = 1u << (lgn - 1 - i);
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 3; i < n; ++i)
    {
        const unsigned int lb = i & -i;
        rev[i] = rev[i ^ lb] | rev[lb];
    }
}
template <class T, class U>
void reverseCopy(T dest[], const U src[], const unsigned int n, const unsigned int cpy)
{
    static const Complex zero {};
    for (unsigned int i = 0; i < cpy; ++i)
        dest[rev[i]] = src[i];
    for (unsigned int i = cpy; i < n; ++i)
        dest[rev[i]] = zero;
}
template <int rat, class T>
void dft(const T src[], const unsigned int slen, Complex dest[], const unsigned int n)
{
    reverseCopy(dest, src, n, slen);
    for (unsigned int step = 2; step <= n; step <<= 1)
    {
        const Complex wm { cos(rat * 2 * pi / step), sin(rat * 2 * pi / step) };
        const unsigned int half = step >> 1;
        for (Complex* blk = dest; blk < dest + n; blk += step)
        {
            Complex w { 1, 0 };
            for (Complex *j = blk, *k = blk + half; j < blk + half; ++j, ++k)
            {
                const Complex u = *j, v = *k * w;
                *j = u + v;
                *k = u - v;
                w *= wm;
            }
        }
    }
}
void fft(const long long a[], const unsigned int la, const long long b[], const unsigned int lb, long long dest[])
{
    static Complex tmpa[maxn + 10], tmpb[maxn + 10];
    const unsigned int lg = Log2(la + lb - 1), n = 1u << lg;
    initRev(lg);
    dft<1>(a, la, tmpa, n);
    dft<1>(b, lb, tmpb, n);
    for (unsigned int i = 0; i < n; ++i)
        tmpb[i] *= tmpa[i];
    dft<(-1)>(tmpb, n, tmpa, n);
    for (unsigned int i = 0; i < n; ++i)
        dest[i] = round(tmpa[i].real() / n);
}
long long solve(const long long a[], const long long b[], const unsigned int n)
{
    static long long reva[maxn + 10], mv[maxn + 10];
    reverse_copy(a, a + n, reva);
    copy(reva, reva + n, reva + n);
    fft(reva, n * 2, b, n, mv);
    long long ret = *max_element(mv + n, mv + n * 2) * (-2), sum = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        ret += a[i] * a[i] + b[i] * b[i];
        sum += a[i] - b[i];
    }
    const auto eval = [n, sum](long long c) { return c * c * n + 2 * c * sum; };
    return ret + min({ eval(-sum / n), eval(-sum / n + 1), eval(-sum / n - 1) });
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<long long>(cin), n, a);
    copy_n(istream_iterator<long long>(cin), n, b);
    cout << solve(a, b, n) << "\n";
    return 0;
}