#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxln = 18, maxn = 1u << maxln;
constexpr unsigned long long mod = 1004535809, primitiveRoot = 3;

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
    inline Number operator-() const
    {
        Number ret;
        ret.val = val ? mod - val : 0;
        return ret;
    }
    inline friend ostream& operator<<(ostream& os, const Number a)
    {
        return os << a.val;
    }
    inline friend istream& operator>>(istream& is, Number& v)
    {
        return is >> v.val;
    }

private:
    unsigned long long val = 0;
};
Number unit[2][maxn + 10], inv[maxn + 10];
unsigned int rev[maxn + 10];

Number readK()
{
    Number ret;
    char ch = cin.get();
    while (!isdigit(ch))
        ch = cin.get();
    while (isdigit(ch))
    {
        ret = ret * 10 + (ch - '0');
        ch = cin.get();
    }
    return ret;
}
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
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(primitiveRoot, (mod - 1) >> i);
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
static void initModInverse(const unsigned int n)
{
    static Number prd[maxn + 10], iprd[maxn + 10];
    prd[0] = prd[1] = 1;
    iprd[0] = 1;
    for (unsigned int i = 2; i <= n; ++i)
        prd[i] = prd[i - 1] * i;
    iprd[n] = quickPow(prd[n], mod - 2);
    for (unsigned int i = n; i; --i)
    {
        iprd[i - 1] = iprd[i] * i;
        inv[i] = iprd[i] * prd[i - 1];
    }
}
static void init(const unsigned int lgn)
{
    initUnit(lgn + 1);
    initRev(lgn + 1);
    initModInverse((1u << lgn) + 1);
}

template <unsigned int invalid>
void reverseCopy(Number dest[], const Number src[], const unsigned int lgn)
{
    static const Number zero;
    const unsigned int cpy = 1u << (lgn - invalid), n = 1u << lgn;
    for (unsigned int i = 0; i < cpy; ++i)
        dest[rev[i]] = src[i];
    for (unsigned int i = cpy; i < n; ++i)
        dest[rev[i]] = zero;
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
template <unsigned int ib>
void multiplyWith(const Number vx[], const Number b[], Number dest[], const unsigned int lgn)
{
    static Number tmpb[maxn + 10];
    dft<false, ib + 1>(b, tmpb, lgn + 1);
    const unsigned int n = 1u << (lgn + 1);
    for (unsigned int i = 0; i < n; ++i)
        tmpb[i] *= vx[i];
    dft<true, 0>(tmpb, dest, lgn + 1);
}

void prefixSum(const Number pOrig[], const Number k, Number dest[], const unsigned int lgn)
{
    static Number mul[maxn + 10];
    const unsigned int n = 1u << lgn;
    mul[0] = 1;
    for (unsigned int i = 1; i < n; ++i)
        mul[i] = mul[i - 1] * (k + i - 1) * inv[i];
    multiplyWith<0>(pOrig, mul, dest, lgn);
}
void difference(const Number pOrig[], const Number k, Number dest[], const unsigned int lgn)
{
    static Number mul[maxn + 10];
    const unsigned int n = 1u << lgn;
    mul[0] = 1;
    for (unsigned int i = 1; i < n; ++i)
        mul[i] = -mul[i - 1] * (k + 1 - i) * inv[i];
    multiplyWith<0>(pOrig, mul, dest, lgn);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, t;
    cin >> n;
    const Number k = readK();
    cin >> t;
    static Number a[maxn + 10], ans[maxn + 10], tmpa[maxn + 10];
    copy_n(istream_iterator<Number>(cin), n, a + 1);
    const unsigned int lg = Log2(n + 1);
    init(lg);
    dft<false, 1>(a, tmpa, lg + 1);
    if (t)
        difference(tmpa, k, ans, lg);
    else
        prefixSum(tmpa, k, ans, lg);
    copy(ans + 1, ans + 1 + n, ostream_iterator<Number>(cout, " "));
    cout.put('\n');
    return 0;
}