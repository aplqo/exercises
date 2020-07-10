#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
constexpr unsigned int maxl = 18;
constexpr unsigned long long mod = 1e9 + 7;

class Number
{
public:
    Number() = default;
    Number(unsigned long long v)
        : val(v % mod)
    {
    }
#define decl(x)                                                      \
    inline friend Number operator x(const Number l, const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
    decl(-);
#undef decl
    friend inline ostream& operator<<(ostream& os, const Number v);

private:
    unsigned long long val = 0;
};
#define impl(x) \
    inline Number operator x(const Number l, const Number r) { return l.val x r.val; }
impl(+);
impl(*);
#undef impl
inline Number operator-(const Number l, const Number r)
{
    return l.val + mod - r.val;
}
inline ostream& operator<<(ostream& os, const Number v)
{
    return os << v.val;
}
Number po[maxl + 10] = { 1 }, f[maxl + 3] = { 0, 0 };

inline void init(const unsigned int l)
{
    for (unsigned int i = 1; i <= l; ++i)
        po[i] = po[i - 1] * 10;
    for (unsigned int i = 1; i <= l; ++i)
        f[i] = po[i] + mod - 1;
}
inline void solve(unsigned long long x, Number dat[10])
{
    unsigned int digit = 0;
    Number rat = 0;
    while (x)
    {
        const unsigned int i = x % 10;
        dat[i] += rat + (i ? 1 : 0);
        for (unsigned int t = 1; t < i; ++t)
            dat[t] += po[digit];
        if (digit)
        {
            for (unsigned int t = 0; t < 10; ++t)
                dat[t] += digit * i * po[digit - 1];
            if (!i)
                dat[0] -= f[digit];
        }
        rat += i * po[digit];
        ++digit;
        x /= 10;
    }
}

int main()
{
    init(maxl + 2);
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        Number cnt[2][10] = {};
        unsigned long long a, b;
        cin >> a >> b;
        solve(a - 1, cnt[0]);
        solve(b, cnt[1]);
        Number ans = 0;
        for (unsigned int i = 1; i < 10; ++i)
            ans += (cnt[1][i] - cnt[0][i]) * i;
        cout << ans << endl;
    }
    return 0;
}