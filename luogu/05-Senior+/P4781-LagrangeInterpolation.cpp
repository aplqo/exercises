#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e3;
constexpr unsigned long long mod = 998244353;

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

private:
    unsigned long long val = 0;
};
inline Number quickPow(Number a, unsigned long long e)
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
inline Number operator/(const Number a, const Number b)
{
    return quickPow(b, mod - 2) * a;
}

Number w[maxn + 10], x[maxn + 10], y[maxn + 10];

void insert(const unsigned int k, const Number cx, const Number cy)
{
    Number acc = 1;
    for (unsigned int i = 0; i < k; ++i)
    {
        w[i] = w[i] / (x[i] - cx);
        acc *= cx - x[i];
    }
    x[k] = cx;
    y[k] = cy;
    w[k] = 1 / acc;
}
Number eval(const unsigned int k, const Number cx)
{
    Number vf = 0, vp = 0;
    for (unsigned int i = 0; i < k; ++i)
    {
        const Number iv = 1 / (cx - x[i]);
        vf += w[i] * y[i] * iv;
        vp += w[i] * iv;
    }
    return vf / vp;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        insert(i, x, y);
    }
    cout << eval(n, k) << "\n";
    return 0;
}