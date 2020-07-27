#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
constexpr unsigned long long mod = 998244353;

unsigned long long phi(unsigned long long n)
{
    const unsigned long long ov = n;
    unsigned long long ret = n;
    for (unsigned long long i = 2; i * i <= ov; ++i)
    {
        if (n % i)
            continue;
        ret = ret / i * (i - 1);
        while (!(n % i))
            n /= i;
    }
    if (n > 1)
        ret = ret / n * (n - 1);
    return ret % mod;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long n, m;
    cin >> n >> m;
    const unsigned long long t1 = phi(n) * phi(m) % mod;
    n %= mod;
    m %= mod;
    cout << ((n * m) % mod * t1) % mod << endl;
    return 0;
}