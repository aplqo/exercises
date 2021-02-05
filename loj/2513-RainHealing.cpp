#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int maxn = 1500;
constexpr unsigned long long mod = 1000000007;

struct Number
{
    inline friend Number operator+(const Number l, const Number r)
    {
        Number ret { l.val + r.val };
        if (ret.val > mod)
            ret.val -= mod;
        return ret;
    }
    inline void operator+=(const Number r)
    {
        val += r.val;
        if (val > mod)
            val -= mod;
    }
    inline friend Number operator*(const Number l, const Number r) { return Number { (l.val * r.val) % mod }; }
    inline void operator*=(const Number r) { val = (val * r.val) % mod; }
    inline friend std::ostream& operator<<(std::ostream& os, const Number v)
    {
        return os << v.val;
    }
    inline Number operator-() const
    {
        return Number { val ? mod - val : 0 };
    }
    inline bool notZero() const { return val; }

    unsigned long long val;
};
Number prod[maxn + 10], ifac[maxn + 10], p[maxn + 10];
Number equ[maxn + 10][maxn + 10];

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
inline Number inverse(const Number v)
{
    return quickPow(v, mod - 2);
}
static void initFactor(const unsigned int n)
{
    static Number fac[maxn + 10], prd[maxn + 10], iprd[maxn + 10];
    fac[0] = iprd[0] = prd[0] = ifac[0] = Number { 1 };
    for (unsigned int i = 1; i <= n; ++i)
    {
        fac[i] = fac[i - 1] * Number { i };
        prd[i] = prd[i - 1] * fac[i];
    }
    iprd[n] = inverse(prd[n]);
    for (unsigned int i = n; i; --i)
    {
        iprd[i - 1] = iprd[i] * fac[i];
        ifac[i] = iprd[i] * prd[i - 1];
    }
}
static void initProduct(const unsigned int n, const unsigned long long k)
{
    prod[0] = Number { 1 };
    for (unsigned int i = 1; i <= n; ++i)
        prod[i] = prod[i - 1] * Number { (k + 1 - i) % mod };
}
static void initProbably(const unsigned int n, const unsigned long long m, const unsigned long long k)
{
    initFactor(n);
    initProduct(n, k);
    p[n] = quickPow(Number { m }, k - n) * inverse(quickPow(Number { m + 1 }, k));
    for (unsigned int i = n - 1; i; --i)
        p[i] = p[i + 1] * Number { m };
    p[0] = p[1] * Number { m };
    for (unsigned int i = 0; i <= n; ++i)
        p[i] *= prod[i] * ifac[i];
}
void buildMatrix(const unsigned int n, const unsigned long long m)
{
    const Number hit = -inverse(Number { m + 1 }), miss = hit * Number { m };
    for (unsigned int i = 0; i <= n + 1; ++i)
        equ[0][i] = Number { 0 };
    for (unsigned int i = 1; i < n; ++i)
    {
        for (unsigned int j = 0; j <= i; ++j)
            equ[i][j] = hit * p[i + 1 - j] + miss * p[i - j];
        equ[i][i + 1] = hit * p[0];
        equ[i][n + 1] = Number { 1 };
        equ[i][i] += Number { 1 };
    }
    equ[0][0] = Number { 1 };
    for (unsigned int i = 0; i <= n; ++i)
        equ[n][i] = -p[n - i];
    equ[n][n] += Number { 1 };
    equ[n][n + 1] = Number { 1 };
}

bool gaussian(const unsigned int n)
{
    for (unsigned int i = 1; i < n; ++i)
    {
        if (!equ[i][i].notZero())
            continue;
        const Number inv = inverse(equ[i][i]);
        for (unsigned int j = i + 1; j <= n; ++j)
        {
            const Number r = -inv * equ[j][i];
            equ[j][i] += r * equ[i][i];
            equ[j][i + 1] += r * equ[i][i + 1];
            equ[j][n + 1] += r * equ[i][n + 1];
        }
    }
    for (unsigned int i = n; i; --i)
    {
        if (equ[i][n + 1].notZero() && !equ[i][i].notZero())
            return false;
        equ[i][n + 1] *= inverse(equ[i][i]);
        equ[i - 1][n + 1] += -equ[i - 1][i] * equ[i][n + 1];
    }
    return true;
}

std::pair<bool, Number> solve(const unsigned int p, const unsigned int n, const unsigned long long m, const unsigned long long k)
{
    if (p && (!k || (!m && k == 1)))
        return std::make_pair(false, Number { 0 });
    std::fill(::p, ::p + n + 1, Number { 0 });
    initProbably(std::min<unsigned long long>(n, k), m, k);
    buildMatrix(n, m);
    if (!gaussian(n) || !equ[p][p].notZero())
        return std::make_pair(false, Number { 0 });
    return std::make_pair(true, equ[p][n + 1]);
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int t;
    std::cin >> t;
    for (unsigned int i = t; i; --i)
    {
        unsigned int n, p;
        unsigned long long m, k;
        std::cin >> n >> p >> m >> k;
        const auto [suc, val] = solve(p, n, m, k);
        if (suc)
            std::cout << val << "\n";
        else
            std::cout << "-1\n";
    }
    return 0;
}
