#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 5000;
constexpr unsigned long long mod = 998244353;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                               \
    friend inline Number operator x(const Number l, const Number r) { return l.val x r.val; } \
    inline Number operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
#undef decl
    friend inline ostream& operator<<(ostream& os, const Number v)
    {
        return os << v.val;
    }

private:
    unsigned long long val;
};
Number fac[maxn + 1], ifac[maxn + 1];

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
static void factor(const unsigned int n)
{
    fac[0] = fac[1] = 1;
    ifac[0] = ifac[1] = 1;
    for (unsigned int i = 2; i <= n; ++i)
    {
        fac[i] = fac[i - 1] * i;
        ifac[i] = quickPow(fac[i], mod - 2);
    }
}
inline Number c(const unsigned int n, const unsigned int m)
{
    return n < m ? 0 : fac[n] * ifac[n - m] * ifac[m];
}
inline Number count(const unsigned int a, const unsigned int b)
{
    const unsigned int mv = min(a, b);
    Number ret = 0;
    for (unsigned int i = 0; i <= mv; ++i)
        ret += c(a, i) * c(b, i) * fac[i];
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int a, b, c;
    cin >> a >> b >> c;
    factor(max({ a, b, c }));
    cout << count(a, b) * count(b, c) * count(a, c) << "\n";
    return 0;
}