#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2000;
constexpr unsigned long long mod = 1e9 + 7;

class Number
{
public:
    Number() = default;
    Number(const unsigned long long v)
        : val(v % mod)
    {
    }
#define decl(x)                                                                                       \
    friend inline Number operator x(const Number l, const Number r) { return (l.val + mod) x r.val; } \
    inline Number operator x##=(const Number r) { return *this = *this x r; }
    decl(+);
    decl(-);
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
inline Number operator/(const Number a, const Number b)
{
    return a * quickPow(b, mod - 2);
}
unsigned int a[maxn + 10];
Number b[maxn + 10], s[maxn + 10];

static void initB(const unsigned int n, const unsigned int k)
{
    b[0] = 1;
    for (unsigned int i = 1; i <= n; ++i)
        b[i] = b[i - 1] * (k + i - 1) / i;
}
static void getAns(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j <= i; ++j)
            s[i] += a[i - j] * b[j];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    copy_n(istream_iterator<unsigned int>(cin), n, a);
    initB(n, k);
    getAns(n);
    copy(s, s + n, ostream_iterator<Number>(cout, " "));
    cout << "\n";
    return 0;
}
