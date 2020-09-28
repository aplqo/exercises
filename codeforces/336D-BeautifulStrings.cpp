#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5;
constexpr unsigned long long mod = 1e9 + 7;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                                       \
    friend inline Number operator x(const Number l, const Number r) { return (l.val + mod) x r.val; } \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
#undef decl
    friend inline ostream& operator<<(ostream& os, const Number v)
    {
        return os << v.val;
    }

private:
    unsigned long long val = 0;
};
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

Number fac[maxn * 2 + 10], ifac[maxn * 2 + 10];

static void getFactor(const unsigned int v)
{
    fac[0] = ifac[0] = 1;
    for (unsigned int i = 1; i <= v; ++i)
    {
        fac[i] = fac[i - 1] * i;
        ifac[i] = quickPow(fac[i], mod - 2);
    }
}
inline Number C(const unsigned int n, const unsigned int m)
{
    return n >= m ? fac[n] * ifac[m] * ifac[n - m] : 0;
}
Number dp(const unsigned int one, const unsigned int zero, const bool val)
{
    if (one + zero <= 1)
        return one == val;
    if (!one || !zero)
        return !zero ? !val : !(zero & 0x01) == val;
    return val ? dp(one, zero - 1, false) : C(one + zero - 1, zero) + dp(one, zero - 1, true);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int zero, one, g;
    cin >> zero >> one >> g;
    getFactor(zero + one);
    cout << dp(one, zero, g) << "\n";
    return 0;
}
