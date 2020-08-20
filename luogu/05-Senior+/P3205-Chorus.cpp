#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1000;
constexpr unsigned long long mod = 19650827;

unsigned int h[maxn + 1];
unsigned long long f[maxn + 1][maxn + 1][2];

unsigned long long dp(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
        f[i][i][0] = f[i][i][1] = 1;
    for (unsigned int i = 1; i < n; ++i)
        for (unsigned int l = 0; l + i < n; ++l)
        {
            const unsigned int r = l + i;
            if (h[l] < h[r])
                f[l][r][1] = (f[l][r][1] + f[l + 1][r][0]) % mod;
            if (h[l] < h[l + 1] && l + 1 != r)
                f[l][r][1] = (f[l][r][1] + f[l + 1][r][1]) % mod;
            if (h[r] > h[r - 1])
                f[l][r][0] = (f[l][r][0] + f[l][r - 1][0]) % mod;
            if (h[r] > h[l] && l != r - 1)
                f[l][r][0] = (f[l][r][0] + f[l][r - 1][1]) % mod;
        }
    return (f[0][n - 1][1] + f[0][n - 1][0]) % mod;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, h);
    cout << dp(n) << endl;
    return 0;
}
