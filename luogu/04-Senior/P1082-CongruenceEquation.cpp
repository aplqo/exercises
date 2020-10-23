#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;

void exgcd(const long long a, const long long b, long long& x, long long& y)
{
    if (!b)
    {
        x = 1;
        y = 0;
        return;
    }
    exgcd(b, a % b, x, y);
    const long long t = x;
    x = y;
    y = t - (a / b) * y;
}
unsigned long long inverse(const long long a, const long long m)
{
    long long x, y;
    exgcd(a, m, x, y);
    return (x % m + m) % m;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long a, m;
    cin >> a >> m;
    cout << inverse(a, m) << "\n";
    return 0;
}