#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5, maxo = 1e6;
constexpr unsigned long long mod = 1e9 + 7;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                                                       \
    inline friend Number operator x(const Number l, const Number r) { return (l.val + mod) x r.val; } \
    inline void operator x##=(const Number r) { val = ((val + mod) x r.val) % mod; }
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

Number f[maxo + 10];

inline void addObject(const unsigned int m, const unsigned long long size)
{
    if (size > m)
        return;
    for (unsigned int i = size; i <= m; ++i)
        f[i] += f[i - size];
}
inline void removeObject(const unsigned int m, const long long size)
{
    for (int i = m; i >= size; --i)
        f[i] -= f[i - size];
}
Number count(const unsigned int n, const unsigned int m)
{
    f[0] = 1;
    static const auto sumN = [](const unsigned long long v) { return v * (v + 1) >> 1; };
    addObject(m, n);
    for (unsigned int i = 1; i < n; ++i)
        addObject(m, sumN(i));
    Number ret;
    for (unsigned int i = 1; i <= n && sumN(i - 1) <= m; ++i)
    {
        ret += f[m - sumN(i - 1)];
        removeObject(m, sumN(n - i));
        addObject(m, sumN(i));
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    cout << count(n, m) << "\n";
    return 0;
}