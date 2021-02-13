#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int maxn = 1e5;
constexpr unsigned long long mod = 998244353, inv3 = 332748118;

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
    inline friend std::ostream& operator<<(std::ostream& os, const Number v)
    {
        return os << v.val;
    }

    unsigned long long val;
};
Number powNat[maxn + 10], powNeg2[maxn + 10];
Number ifac[maxn + 10], fac[maxn + 10], inv[maxn + 10];

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
static void eular(const unsigned int n)
{
    static unsigned int primes[maxn + 10], *pcur = primes;
    static unsigned int mindiv[maxn + 10];
    powNat[0].val = powNat[1].val = 1;
    for (unsigned long long i = 2; i <= n; ++i)
    {
        if (!mindiv[i])
        {
            powNat[i] = quickPow(Number { i }, n + 1);
            mindiv[i] = i;
            *(pcur++) = i;
        }
        for (const unsigned int* j = primes; j < pcur && *j <= mindiv[i] && *j * i <= n; ++j)
        {
            const unsigned v = *j * i;
            powNat[v] = powNat[i] * powNat[*j];
            mindiv[v] = *j;
        }
    }
}
static void initPowNeg2(const unsigned int n)
{
    const Number p { mod - 2 };
    powNeg2[0].val = 1;
    for (unsigned int i = 1; i <= n; ++i)
        powNeg2[i] = powNeg2[i - 1] * p;
}
static void init(const unsigned int n)
{
    initModularInv(n);
    initFactor(n);
    initPowNeg2(n + 1);
    eular(n);
}

inline Number C(const unsigned int n, const unsigned int m)
{
    return fac[n] * ifac[n - m] * ifac[m];
}
inline Number seriesSum(const unsigned int k, const unsigned int n)
{
    switch (k)
    {
    case 0:
        return Number { 1 };
    case 1:
        return Number { n + 1 };
    default:
        return (powNat[k] - Number { 1 }) * inv[k - 1];
    }
}
Number sum(const unsigned int n)
{
    Number ret {};
    Number acc { 1 }, g = (Number { 1 } - quickPow(Number { mod - 2 }, n + 1)) * Number { inv3 };
    for (unsigned int i = 0;;)
    {
        ret += seriesSum(i, n) * acc * g;
        if (i == n)
            break;
        ++i;
        g = (g - powNeg2[n + 1 - i] * (C(n, i) + C(n, i - 1))) * Number { inv3 };
        acc *= Number { 2 };
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n;
    std::cin >> n;
    init(n);
    std::cout << sum(n) << "\n";
    return 0;
}