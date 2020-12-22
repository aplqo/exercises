#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxln = 18, maxn = 1u << maxln;
constexpr unsigned long long mod = 998244353, primeRoot = 3;

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
Number unit[2][maxln + 10], inv[maxn + 10];
unsigned int rev[maxn + 10];

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
static void initUnit(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
        unit[1][i] = quickPow(unit[0][i], mod - 2);
    }
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
static void initMInv(const unsigned int n)
{
    static Number prd[maxn + 10], iprd[maxn + 10];
    prd[0] = prd[1] = 1;
    for (unsigned int i = 2; i <= n; ++i)
        prd[i] = prd[i - 1] * i;
    iprd[n] = quickPow(prd[n], mod - 2);
    inv[0] = 1;
    for (unsigned int i = n; i; --i)
    {
        inv[i] = prd[i - 1] * iprd[i];
        iprd[i - 1] = iprd[i] * i;
    }
}
unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 0;; ++i)
        if (n <= (1u << i))
            return i;
    return 0;
}
template <unsigned int invalid>
void reverseCopy(Number dest[], const Number src[], const unsigned int n)
{
    static const Number zero;
    const unsigned int cpy = n >> invalid;
    for (unsigned int i = 0; i < cpy; ++i)
        dest[rev[i]] = src[i];
    for (unsigned int i = cpy; i < n; ++i)
        dest[rev[i]] = zero;
}
template <unsigned int typ, unsigned int invalid>
void dft(const Number a[], Number dest[], const unsigned int lgn)
{
    const unsigned int n = 1u << lgn;
    reverseCopy<invalid>(dest, a, n);
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
        for (Number* i = dest; i < dest + n; ++i)
            *i *= iv;
    }
}
template <unsigned int ia, unsigned int ib, class Func>
void convolution(const Number a[], const Number b[], Number dest[], const unsigned int lgn, Func pred)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    dft<0, ia + 1>(a, tmpa, lgn + 1);
    dft<0, ib + 1>(b, tmpb, lgn + 1);
    const unsigned int n = 1u << (lgn + 1);
    for (unsigned int i = 0; i < n; ++i)
        tmpb[i] = pred(tmpa[i], tmpb[i]);
    dft<1, 0>(tmpb, dest, lgn + 1);
}

inline void ntt(const Number a[], const Number b[], Number dest[], const unsigned int lgn)
{
    convolution<0, 0>(a, b, dest, lgn, [](Number a, Number b) { return a * b; });
}
inline void inverse(const Number a[], Number dest[], const unsigned int lgn)
{
    dest[0] = quickPow(a[0], mod - 2);
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        initRev(i + 1);
        convolution<0, 1>(a, dest, dest, i, [](Number a, Number b) { return b * (2 - a * b); });
    }
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

void ln(const Number a[], Number dest[], const unsigned int lgn)
{
    static Number tp[maxn + 10], iv[maxn + 10];
    const unsigned int n = 1u << lgn;
    derivative(a, tp, n);
    inverse(a, iv, lgn);
    initRev(lgn + 1);
    ntt(tp, iv, iv, lgn);
    integral(iv, dest, n);
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
    const unsigned int l = Log2(n);
    initUnit(l + 1);
    initMInv(1u << l);
    ln(a, res, l);
    copy(res, res + n, ostream_iterator<Number>(cout, " "));
    cout.put('\n');
    return 0;
}