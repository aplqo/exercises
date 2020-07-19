#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 10000, maxk = 10;

static unsigned long long f[maxn * maxk + 1][maxk + 1];
unsigned long long solve(const unsigned int n, const unsigned int k, const unsigned long long mod)
{
    memset(f, 0, sizeof(f));
    f[0][0] = 1;
    for (unsigned int i = 1; i <= n * k; ++i)
        for (unsigned int j = 1; j <= k; ++j)
        {
            if (i >= j)
                f[i][j] = (f[i - j][j - 1] + f[i - j][j]) % mod;
            if (i >= n + 1)
                f[i][j] = (f[i][j] + mod - f[i - (n + 1)][j - 1]) % mod;
        }
    unsigned long long ret = 0;
    for (unsigned int j = 0; j <= n * k; ++j)
        for (unsigned int i = 0; i <= k; ++i)
        {
            ret = (ret + (f[j][i] * f[j][k - i]) % mod) % mod;
            if (k - i >= 1)
                ret = (ret + (f[j][i] * f[j][k - i - 1]) % mod) % mod;
        }
    return ret;
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
        unsigned int n, k, p;
        cin >> n >> k >> p;
        cout << solve(n, k, p) << endl;
    }
    return 0;
}