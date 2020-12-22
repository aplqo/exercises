#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxln = 18, maxn = 1u << maxln;
constexpr unsigned long long mod = 998244353, primeRoot = 3, inv2 = 499122177;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                                       \
    inline friend Number operator x(const Number l, const Number r) { return (l.val + mod) x r.val; } \
    inline void operator x##=(const Number r) { *this = *this x r; }
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

private:
    unsigned long long val = 0;
};
Number unit[2][maxln + 10];
unsigned int rev[maxln + 10][maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
    Number ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}
unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 0;; ++i)
        if (n <= (1u << i))
            return i;
    return 0;
}
static void initUnit(const unsigned int lgn)
{
    for (unsigned int i = 0; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
        unit[1][i] = quickPow(unit[0][i], mod - 2);
    }
}
static void initRev(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unsigned int* const crev = rev[i];
        for (unsigned int j = 0; j < i; ++j)
            crev[1u << j] = 1u << (i - 1 - j);
        const unsigned int n = 1u << i;
        for (unsigned int j = 0; j < n; ++j)
        {
            const unsigned int lb = j & -j;
            crev[j] = crev[j ^ lb] | crev[lb];
        }
    }
}
template <unsigned int invalid>
void reverseCopy(Number dest[], const Number src[], const unsigned int lgn)
{
    static const Number zero;
    const unsigned int lim = 1u << (lgn - invalid), n = 1u << lgn;
    const unsigned int* const crev = rev[lgn];
    for (unsigned int i = 0; i < lim; ++i)
        dest[crev[i]] = src[i];
    for (unsigned int i = lim; i < n; ++i)
        dest[crev[i]] = zero;
}
template <unsigned int typ, unsigned int invalid>
void dft(const Number a[], Number dest[], const unsigned int lgn)
{
    const unsigned int n = 1u << lgn;
    reverseCopy<invalid>(dest, a, lgn);
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        const unsigned int step = 1u << i, half = 1u << (i - 1);
        const Number wm = unit[typ][i];
        for (unsigned int blk = 0; blk < n; blk += step)
        {
            Number w = 1;
            for (unsigned int i = 0; i < half; ++i)
            {
                const Number u = dest[blk + i], v = w * dest[blk + i + half];
                dest[blk + i] = u + v;
                dest[blk + i + half] = u - v;
                w *= wm;
            }
        }
    }
    if constexpr (typ)
    {
        const Number iv = quickPow(n, mod - 2);
        for (unsigned int i = 0; i < n; ++i)
            dest[i] *= iv;
    }
}
template <unsigned int iva, unsigned int ivb, class Func>
void convolution(const Number a[], const Number b[], Number out[], const unsigned int lgn, Func pred)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    dft<0, iva + 1>(a, tmpa, lgn + 1);
    dft<0, ivb + 1>(b, tmpb, lgn + 1);
    const unsigned int n = 1u << (lgn + 1);
    for (unsigned int i = 0; i < n; ++i)
        tmpb[i] = pred(tmpa[i], tmpb[i]);
    dft<1, 0>(tmpb, out, lgn + 1);
}

inline void ntt(const Number a[], const Number b[], Number out[], const unsigned int lgn)
{
    convolution<0, 0>(a, b, out, lgn, multiplies<Number>());
}
template <unsigned int ia, unsigned int ib>
inline void updateInverse(const Number a[], const Number old[], Number out[], const unsigned int lgn)
{
    convolution<ia, 1 + ib>(a, old, out, lgn, [](Number a, Number b) { return b * (2 - a * b); });
}
void sqrt(const Number a[], Number out[], const unsigned int lgn)
{
    static Number inv[maxn + 10], tmp[maxn + 10], tinv[maxn + 10];
    inv[0] = out[0] = 1;
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        updateInverse<1, 0>(out, inv, tinv, i);
        ntt(tinv, a, tmp, i);
        {
            const unsigned int n = 1u << i;
            for (unsigned int j = 0; j < n; ++j)
                out[j] = inv2 * (out[j] + tmp[j]);
        }
        updateInverse<0, 0>(out, inv, inv, i);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    static Number a[maxn + 10], res[maxn + 10];
    cin >> n;
    copy_n(istream_iterator<Number>(cin), n, a);
    const unsigned int l = Log2(n);
    initUnit(l + 1);
    initRev(l + 1);
    sqrt(a, res, l);
    copy(res, res + n, ostream_iterator<Number>(cout, " "));
    cout.put('\n');
    return 0;
}