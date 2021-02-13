#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxk = 5000;
constexpr unsigned long long mod = 998244353;

class Number
{
public:
#define decl(x)                                                                                                  \
    inline friend Number operator x(const Number l, const Number r) { return Number { (l.val x r.val) % mod }; } \
    inline void operator x##=(const Number r) { val = (val x r.val) % mod; }
    decl(+);
    decl(*);
#undef decl
    inline friend std::ostream& operator<<(std::ostream& os, const Number v)
    {
        return os << v.val;
    }

    unsigned long long val;
};

Number s[maxk + 10][maxk + 10];

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
static void initStirling(const unsigned int k)
{
    s[0][0].val = s[1][1].val = 1;
    for (unsigned int i = 2; i <= k; ++i)
        for (unsigned int j = 1; j <= i; ++j)
            s[i][j] = s[i - 1][j - 1] + s[i - 1][j] * Number { j };
}
Number solve(const unsigned int n, const unsigned int m, const unsigned int k)
{
    const Number inv = quickPow(Number { m }, mod - 2);
    Number ret {}, accN { n }, accM { inv };
    for (unsigned int i = 1; i <= k; ++i)
    {
        ret += s[k][i] * accN * accM;
        accN *= Number { n - i };
        accM *= inv;
    }
    return ret;
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n, m, k;
    std::cin >> n >> m >> k;
    initStirling(k);
    std::cout << solve(n, m, k) << "\n";
    return 0;
}