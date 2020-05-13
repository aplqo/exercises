/*luogu team T133430: Jump*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
constexpr unsigned long long mod = 1e9 + 7;

class Number
{
public:
    Number() = default;
    Number(unsigned long long i)
        : val(i % mod)
    {
    }
#define alop(o)                                                            \
    inline Number operator o(const Number r) const { return val o r.val; } \
    inline Number& operator o##=(const Number r) { return *this = *this o r; }
    alop(+);
    alop(*);
#undef alop

    friend inline ostream& operator<<(ostream& os, const Number v);

private:
    unsigned long long val = 0;
};
inline Number operator^(Number a, unsigned long long e)
{
    Number ret = 1;
    for (unsigned long long i = 1; e; i <<= 1)
    {
        if (e & i)
        {
            ret *= a;
            e ^= i;
        }
        a *= a;
    }
    return ret;
}
inline Number operator/(const Number l, const Number r)
{
    Number inv = r ^ (mod - 2);
    return l * inv;
}
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }
Number factor(const unsigned long long n)
{
    Number ret = 1;
    for (unsigned long long i = 1; i <= n; ++i)
        ret *= i;
    return ret;
}

Number c(unsigned long long n, unsigned long long m)
{
    const static auto co = [](const unsigned long long i,
                               const unsigned long long j) {
        return factor(j) / (factor(j - i) * factor(i));
    };
    Number ret = 1;
    while (n && m)
    {
        ret *= co(n % mod, m % mod);
        n /= mod;
        m /= mod;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long n, m;
    cin >> n >> m;
    if (n < m)
        swap(n, m);
    Number ans = n + 1;
    for (unsigned int i = 1; i <= m; ++i)
        ans += c(n, i + n);
    cout << ans << endl;
    return 0;
}