#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 4200;

class Number
{
public:
    Number() = default;
    Number(unsigned long long v)
        : val(v % mod) {};
#define decl(x)                                                      \
    friend inline Number operator x(const Number l, const Number r); \
    inline Number& operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(*);
#undef decl
    friend inline ostream& operator<<(ostream& os, const Number v);

    static unsigned long long mod;

private:
    unsigned long long val = 0;
};
unsigned long long Number::mod;
#define impl(x) \
    inline Number operator x(const Number l, const Number r) { return l.val x r.val; }
impl(+);
impl(*);
#undef impl
inline ostream& operator<<(ostream& os, const Number v)
{
    return os << v.val;
}

Number c[maxn + 1][maxn + 1];

static void initC(const unsigned int n)
{
    c[0][0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
    {
        c[i][0] = 1;
        for (unsigned int j = 1; j <= i; ++j)
            c[i][j] = c[i - 1][j] + c[i - 1][j - 1];
    }
}
Number dp(const unsigned int n)
{
    static Number f[maxn + 1];
    f[0] = f[1] = 1;
    for (unsigned int i = 2; i <= n; ++i)
        for (unsigned int j = 2; j <= i; j += 2)
            f[i] += f[j - 1] * f[i - j] * c[i - 1][j - 1];
    return f[n] * 2;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> Number::mod;
    initC(n);
    cout << dp(n) << "\n";
    return 0;
}