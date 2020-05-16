/*Luogu team T133193: Treasure hunt team*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
constexpr unsigned long long mod = 1e9 + 7;

struct Number
{
    Number() = default;
    Number(const unsigned long long i)
        : val(i % mod)
    {
    }
#define decl(x) inline Number& operator x##=(const Number r);
    decl(+);
    decl(*);
    decl(/);
#undef decl

    unsigned long long val = 0;
};
#define impl(x) \
    inline Number operator x(const Number l, const Number r) { return l.val x r.val; }
impl(+);
impl(*);
#undef impl
Number operator^(Number a, unsigned long long e)
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
inline Number operator/(const Number a, const Number b)
{
    return a * (b ^ (mod - 2));
}
#define assop(x) \
    inline Number& Number::operator x##=(const Number r) { return *this = *this x r; }
assop(+);
assop(*);
assop(/);
#undef assop
inline ostream& operator<<(ostream& os, const Number r)
{
    return os << r.val;
}
Number factor(const unsigned long long a, const unsigned long long b)
{
    Number ret = 1;
    for (unsigned long long i = b; i > a; --i)
        ret *= i;
    return ret;
}
Number solve(const unsigned int n, const unsigned long long m)
{
    Number ret = 0;
    const unsigned long long p = (m - 1) >> 1;
    const Number facp = factor(0, p);
    Number v = facp * factor(n - p * 2 - 1, n - p - 1);
    for (unsigned long long i = p + 1; i <= n / 2; ++i)
    {
        ret += (n + 1) * v;
        v *= i * (n - i - p) / (i - p) / (n - i);
    }
    if (n % 2)
        ret += ((n + 1) >> 1) * v;
    ret /= facp * facp;
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long n, m;
    cin >> n >> m;
    cout << solve(n, m) << endl;
    return 0;
}