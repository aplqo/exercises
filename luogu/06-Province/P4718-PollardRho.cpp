#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
using namespace std;

unsigned long long multiply(unsigned long long a, unsigned long long b, const unsigned long long mod)
{
    unsigned long long ret = 0;
    for (; b; b >>= 1)
    {
        if (b & 0x01)
            ret = (ret + a) % mod;
        a = (a + a) % mod;
    }
    return ret;
}
unsigned long long quickPow(unsigned long long a, unsigned long long e, const unsigned long long mod)
{
    unsigned long long ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = multiply(ret, a, mod);
        a = multiply(a, a, mod);
    }
    return ret;
}
pair<unsigned int, unsigned long long> lowbit(unsigned long long n)
{
    --n;
    unsigned int ret = 0;
    while (!(n & 0x01))
    {
        ++ret;
        n >>= 1;
    }
    return make_pair(ret, n);
}
bool millerRabin(const unsigned long long n)
{
    constexpr unsigned int testA[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
    const auto [r, d] = lowbit(n);
    for (unsigned int a : testA)
    {
        if (a > n - 2)
            break;
        unsigned long long x = quickPow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        for (unsigned int i = 1; i < r; ++i)
        {
            x = multiply(x, x, n);
            if (x == n - 1)
                goto End;
        }
        return false;
    End:;
    }
    return true;
}
unsigned long long getRandom()
{
    static mt19937 rnd((random_device())());
    static uniform_int_distribution<unsigned long long> dis(1);
    return dis(rnd);
}
unsigned long long pollardRho(const unsigned long long n)
{
    static const auto f = [](const unsigned long long x, const unsigned long long c, const unsigned long long mod) {
        return (multiply(x, x, mod) + c) % mod;
    };
    unsigned long long x = getRandom() % n, y = x, c = getRandom() % n;
    do
    {
        x = f(x, c, n);
        y = f(f(y, c, n), c, n);
        const unsigned long long g = x == y ? gcd(x, n) : gcd(x > y ? x - y : y - x, n);
        if (g != 1)
            return g;
    } while (x != y);
    return 1;
}
unsigned long long factor(const unsigned long long n)
{
    if (n == 1 || millerRabin(n))
        return n;
    unsigned long long v = 1;
    while (v == 1)
        v = pollardRho(n);
    return max(factor(v), factor(n / v));
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
        unsigned long long v;
        cin >> v;
        const unsigned long long f = factor(v);
        if (f == v)
            cout << "Prime\n";
        else
            cout << f << "\n";
    }
    return 0;
}