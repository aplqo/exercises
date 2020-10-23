#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
using namespace std;

unsigned long long countRD(const unsigned long long r, const unsigned long long d)
{
    const unsigned long long v = 2 * r / d;
    unsigned long long ret = 0;
    for (unsigned long long s = 1; s * s <= v; ++s)
    {
        const unsigned long long t = sqrt(v - s * s);
        if (t * t + s * s == v && gcd(t, s) == 1 && t > s)
        {
            const unsigned long long x = (t * t - s * s) / 2 * d, y = d * s * t;
            if (x * x + y * y == r * r)
                ret += 2;
        }
    }
    return ret;
}
unsigned long long count(const unsigned long long r)
{
    const unsigned long long r2 = r * 2;
    unsigned long long ret = 0;
    for (unsigned long long i = 1; i * i <= r2; ++i)
        if (r2 % i == 0)
            ret += countRD(r, i) + (r2 / i != i ? countRD(r, r2 / i) : 0);
    return (ret + 1) * 4;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long r;
    cin >> r;
    cout << count(r) << "\n";
    return 0;
}