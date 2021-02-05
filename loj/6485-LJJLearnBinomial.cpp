#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <iostream>
constexpr unsigned long long mod = 998244353;
constexpr unsigned long long inv4 = 748683265, W = 911660635, invW = 86583718;

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
Number count(const unsigned long long n, const unsigned int s, const unsigned int a[])
{
    Number ret {}, sum[4] {};
    for (unsigned int i = 0; i < 4; ++i)
    {
        const Number f = quickPow(quickPow(Number { W }, i) * Number { s } + Number { 1 }, n);
        const Number p = quickPow(Number { invW }, i);
        Number acc { 1 };
        for (unsigned int j = 0; j < 4; ++j)
        {
            sum[j] += acc * f;
            acc *= p;
        }
    }
    for (unsigned int i = 0; i < 4; ++i)
        ret += sum[i] * Number { a[i] };
    return ret * Number { inv4 };
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int t;
    std::cin >> t;
    for (unsigned int i = t; i; --i)
    {
        unsigned int a[4], s;
        unsigned long long n;
        std::cin >> n >> s >> a[0] >> a[1] >> a[2] >> a[3];
        std::cout << count(n, s, a) << "\n";
    }
    return 0;
}