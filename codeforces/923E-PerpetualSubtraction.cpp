#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxl = 18, maxn = 1u << maxl;
constexpr unsigned long long mod = 998244353, primitiveRoot = 3;

class Number
{
public:
#define decl(x)                                                                                                  \
    inline friend Number operator x(const Number l, const Number r) { return Number { (l.val x r.val) % mod }; } \
    inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
    decl(+);
    decl(*);
#undef decl
    inline friend Number operator-(const Number l, const Number r)
    {
        return Number { l.val >= r.val ? l.val - r.val : l.val + mod - r.val };
    }
    inline void operator-=(const Number r) { val = val >= r.val ? val - r.val : val + mod - r.val; }
    inline friend std::ostream& operator<<(std::ostream& os, const Number v) { return os << v.val; }
    inline Number operator-() const { return Number { val ? mod - val : 0 }; }

    unsigned long long val;
};
Number unit[2][maxl + 10], fac[maxn + 10], ifac[maxn + 10], inv[maxn + 10];
unsigned int rev[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
    Number ret { 1 };
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= a;
        a *= a;
    }
    return ret;
}
inline Number inverse(const Number a)
{
    return quickPow(a, mod - 2);
}
static void initUnit(const unsigned int lgn)
{
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        unit[0][i] = quickPow(Number { primitiveRoot }, (mod - 1) >> i);
        unit[1][i] = inverse(unit[0][i]);
    }
}
static void initModularInv(const unsigned int n)
{
    inv[0].val = inv[1].val = 1;
    for (unsigned int i = 2; i <= n; ++i)
        inv[i].val = mod - (inv[mod % i] * Number { mod / i }).val;
}
static void initFactor(const unsigned int n)
{
    fac[0].val = ifac[0].val = 1;
    for (unsigned int i = 1; i <= n; ++i)
    {
        fac[i] = fac[i - 1] * Number { i };
        ifac[i] = ifac[i - 1] * inv[i];
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
unsigned int Log2(const unsigned int n)
{
    for (unsigned int i = 0;; ++i)
        if (n <= (1u << i))
            return i;
}
static unsigned int init(const unsigned int n)
{
    const unsigned int lgn = Log2(n + 1), v = 1u << (lgn + 1);
    initRev(lgn + 1);
    initUnit(lgn + 1);
    initModularInv(v);
    initFactor(v);
    return lgn;
}

template <unsigned int inv>
void reverseCopy(Number dest[], const Number src[], const unsigned int n)
{
    const unsigned int cpy = n >> inv;
    for (unsigned int i = 0; i < cpy; ++i)
        dest[rev[i]] = src[i];
    for (unsigned int i = cpy; i < n; ++i)
        dest[rev[i]].val = 0;
}
template <bool typ, unsigned int inv>
void dft(const Number a[], Number dest[], const unsigned int lgn)
{
    const unsigned int n = 1u << lgn;
    reverseCopy<inv>(dest, a, n);
    for (unsigned int i = 1; i <= lgn; ++i)
    {
        const unsigned int step = 1u << i, half = 1u << (i - 1);
        const Number wm = unit[typ][i];
        for (Number* blk = dest; blk < dest + n; blk += step)
        {
            Number w { 1 };
            for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k)
            {
                const Number u = *j, v = *k * w;
                *j = u + v;
                *k = u - v;
                w *= wm;
            }
        }
    }
    if constexpr (typ)
    {
        const Number iv = ::inv[n];
        for (Number* i = dest; i < dest + n; ++i)
            *i *= iv;
    }
}
template <unsigned int iva = 0, unsigned int ivb = 0>
void ntt(const Number a[], const Number b[], Number dest[], const unsigned int lgn)
{
    static Number tmpa[maxn + 10], tmpb[maxn + 10];
    dft<false, iva + 1>(a, tmpa, lgn + 1);
    dft<false, ivb + 1>(b, tmpb, lgn + 1);
    const unsigned int n = 1u << (lgn + 1);
    for (unsigned int i = 0; i < n; ++i)
        tmpa[i] *= tmpb[i];
    dft<true, 0>(tmpa, dest, lgn + 1);
}

template <bool inv>
void multiplyMatrix(const Number a[], const unsigned int n, const unsigned int lgn, Number dest[])
{
    static Number u[maxn + 10];
    for (unsigned int i = 0; i <= n; ++i)
        if constexpr (inv)
            u[n - i] = fac[i] * a[i];
        else
            u[n - i].val = i & 0x01 ? mod - (fac[i] * a[i]).val : (fac[i] * a[i]).val;
    ntt(u, ifac, dest, lgn);
    std::reverse(dest, dest + n + 1);
    if constexpr (inv)
    {
        for (unsigned int i = 0; i <= n; ++i)
            dest[i] *= ifac[i];
    }
    else
    {
        for (unsigned int i = 0; i <= n; ++i)
            dest[i] = i & 0x01 ? -(dest[i] * ifac[i]) : dest[i] * ifac[i];
    }
}

void solve(const Number ini[], const unsigned int n, const unsigned long long m, const unsigned int lgn, Number dest[])
{
    static Number tmp[maxn + 10];
    multiplyMatrix<true>(ini, n, lgn, tmp);
    for (unsigned int i = 0; i <= n; ++i)
        tmp[i] *= quickPow(inv[i + 1], m);
    multiplyMatrix<false>(tmp, n, lgn, dest);
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n;
    unsigned long long m;
    std::cin >> n >> m;
    static Number ini[maxn + 10];
    for (unsigned int i = 0; i <= n; ++i)
        std::cin >> ini[i].val;
    solve(ini, n, m, init(n), ini);
    std::copy(ini, ini + n + 1, std::ostream_iterator<Number>(std::cout, " "));
    std::cout.put('\n');
    return 0;
}