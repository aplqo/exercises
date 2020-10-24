#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
constexpr unsigned long long mod = 998244353;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
    friend inline Number operator*(const Number l, const Number r) { return l.val * r.val; }
    inline Number operator*=(const Number r) { return val = (val * r.val) % mod; }

    friend inline ostream& operator<<(ostream& os, const Number v)
    {
        return os << v.val;
    }

private:
    unsigned long long val = 0;
};
Number quickPow(Number l, unsigned long long e)
{
    Number ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret *= l;
        l *= l;
    }
    return ret;
}

namespace bf
{
    pair<Number, Number> dfs(const unsigned int n, const unsigned long long dep, const unsigned int g, const Number v)
    {
        if (!dep)
            return make_pair(v, g);
        pair<Number, Number> ret { 1, 1 };
        for (unsigned int i = 1; i <= n; ++i)
        {
            const auto [v1, g1] = dfs(n, dep - 1, (g > 1 ? gcd(g, i) : 1), v * i);
            ret.first *= v1;
            ret.second *= g1;
        }
        return ret;
    }
    Number solve(const unsigned int n, const unsigned long long k)
    {
        pair<Number, Number> ret { 1, 1 };
        for (unsigned int i = 1; i <= n; ++i)
        {
            const auto [v1, g1] = dfs(n, k - 1, i, i);
            ret.first *= v1;
            ret.second *= g1;
        }
        return ret.first * quickPow(ret.second, mod - 2);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        unsigned int n;
        unsigned long long k;
        cin >> n >> k;
        cout << [](unsigned int n, unsigned long long k) { return bf::solve(n, k); }(n, k) << "\n";
    }
    return 0;
}