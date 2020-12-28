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
    inline void operator x##=(const Number r) { val = ((val + mod) x r.val) % mod; }
    decl(+);
    decl(-);
    decl(*);
#undef decl
    inline friend ostream& operator<<(ostream& os, const Number v)
    {
        return os << v.val;
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
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unsigned int* const crev = rev[i];
        for (unsigned int j = 0; j < i; ++j)
            crev[1u << j] = 1u << (i - 1 - j);
        const unsigned int n = 1u << i;
        for (unsigned int j = 3; j < n; ++j)
        {
            const unsigned int lb = j & -j;
            crev[j] = crev[j ^ lb] | crev[lb];
        }
    }
}
unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 0;; ++i)
        if (n <= (1u << i))
            return i;
    return 0;
}
static void init(const unsigned int lgn)
{
    initRev(lgn);
    initUnit(lgn);
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
template <bool inv, unsigned int invalid>
void dft(const Number a[], Number dest[], const unsigned int lgn)
{
    reverseCopy<invalid>(dest, a, lgn);
    const unsigned int n = 1u << lgn;
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        const unsigned int step = 1u << i, half = 1u << (i - 1);
        const Number wm = unit[inv][i];
        for (Number* blk = dest; blk < dest + n; blk += step)
        {
            Number w = 1;
            for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k)
            {
                const Number u = *j, v = *k * w;
                *j = u + v;
                *k = u - v;
                w *= wm;
            }
        }
    }
    if constexpr (inv)
    {
        const Number iv = quickPow(n, mod - 2);
        for (Number* i = dest; i < dest + n; ++i)
            *i *= iv;
    }
}
template <unsigned int ia, unsigned int ib, class Func>
void convolution(const Number a[], const Number b[], Number dest[], const unsigned int lgn, const Func pred)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    dft<false, ia + 1>(a, tmpa, lgn + 1);
    dft<false, ib + 1>(b, tmpb, lgn + 1);
    const unsigned int n = 1u << (lgn + 1);
    for (unsigned int i = 0; i < n; ++i)
        tmpb[i] = pred(tmpa[i], tmpb[i]);
    dft<true, 0>(tmpb, dest, lgn + 1);
}

template <unsigned int ia, unsigned int ib>
inline void ntt(const Number a[], const Number b[], Number dest[], const unsigned int lgn)
{
    convolution<ia, ib>(a, b, dest, lgn, multiplies<Number>());
}
template <unsigned int ia, unsigned int ib>
inline void updateInverse(const Number a[], const Number old[], Number dest[], const unsigned int lgn)
{
    convolution<ia, ib + 1>(a, old, dest, lgn, [](Number a, Number b) { return b * (2 - a * b); });
}
template <unsigned int ia>
void inverse(const Number a[], Number dest[], const unsigned int lgn)
{
    static_assert(ia <= 1);
    dest[0] = quickPow(a[0], mod - 2);
    for (unsigned int i = 1; i < lgn; ++i)
        updateInverse<0, 0>(a, dest, dest, i);
    updateInverse<ia, 0>(a, dest, dest, lgn);
}

void sqrt(const Number a[], Number dest[], const unsigned int lgn)
{
    static Number inv[maxn + 10], tmp[maxn + 10], tinv[maxn + 10];
    inv[0] = dest[0] = 1;
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        updateInverse<1, 0>(dest, inv, tinv, i);
        ntt<0, 0>(tinv, a, tmp, i);
        {
            const unsigned int n = 1u << i, half = 1u << (i - 1);
            for (unsigned int i = 0; i < half; ++i)
                dest[i] = inv2 * (dest[i] + tmp[i]);
            for (unsigned int i = half; i < n; ++i)
                dest[i] = inv2 * tmp[i];
        }
        updateInverse<0, 0>(dest, inv, inv, i);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    static Number c[maxn + 10], sq[maxn + 10], ans[maxn + 10];
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int p;
        cin >> p;
        c[p] = mod - 4;
    }
    c[0] = 1;
    const unsigned int lg = Log2(m + 1);
    init(lg + 1);
    sqrt(c, sq, lg);
    sq[0] += 1;
    inverse<0>(sq, ans, lg);
    for (unsigned int i = 1; i <= m; ++i)
        cout << ans[i] * 2 << "\n";
    return 0;
}