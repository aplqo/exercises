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
Number unit[2][maxln + 10];
unsigned int rev[maxln + 10][maxn + 10], lg[maxn + 10];
Number f[maxn + 10], g[maxn + 10];

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
static void initLog(const unsigned int lgn)
{
    for (unsigned int i = 0; i <= lgn; ++i)
        lg[1u << i] = i;
}
static void initUnit(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(primeRoot, (mod - 1) >> i);
        unit[1][i] = quickPow(unit[0][i], mod - 2);
    }
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
template <unsigned int ia, unsigned int ib>
void ntt(const Number a[], const Number b[], Number dest[], const unsigned int lgn)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    dft<0, 1 + ia>(a, tmpa, lgn + 1);
    dft<0, 1 + ib>(b, tmpb, lgn + 1);
    const unsigned int n = 1u << (lgn + 1);
    for (unsigned int i = 0; i < n; ++i)
        tmpb[i] *= tmpa[i];
    dft<1, 0>(tmpb, dest, lgn + 1);
}
static void init(const unsigned int lgn)
{
    initUnit(lgn);
    initRev(lgn);
    initLog(lgn);
}

void cdq(Number* const pl, Number* const pr)
{
    static Number tmp[maxn + 10];
    if (pr - pl < 2)
        return;
    Number* const pm = pl + ((pr - pl) >> 1);
    cdq(pl, pm);
    ntt<1, 0>(pl, g, tmp, lg[pr - pl]);
    {
        const unsigned int sz = pr - pm;
        for (unsigned int i = 0; i < sz; ++i)
            pm[i] += tmp[sz + i];
    }
    cdq(pm, pr);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<Number>(cin), n - 1, g + 1);
    f[0] = 1;
    const unsigned int l = Log2(n), len = 1u << l;
    init(l + 1);
    cdq(f, f + len);
    copy(f, f + n, ostream_iterator<Number>(cout, " "));
    cout.put('\n');
    return 0;
}