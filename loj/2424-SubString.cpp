#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000, maxm = 200, maxk = maxm;
constexpr unsigned long long mod = 1000000007;

char strA[maxn + 1], strB[maxm + 1];

unsigned long long dp(const unsigned int n, const unsigned int m, const unsigned int k)
{
    static unsigned long long tail[maxm + 1][maxk + 1], sub[maxm + 1][maxk + 1];
    for (unsigned int i = 1; i <= n; ++i)
    {
        for (unsigned int j = m; j >= 2; --j)
        {
            const unsigned int mk = min(j, m);
            for (unsigned int k = mk; k >= 1; --k)
            {
                tail[j][k] = strA[i] == strB[j] ? (sub[j - 1][k - 1] + tail[j - 1][k]) % mod : 0;
                sub[j][k] = (sub[j][k] + tail[j][k]) % mod;
            }
        }
        tail[1][1] = strA[i] == strB[1];
        sub[1][1] = (sub[1][1] + tail[1][1]) % mod;
    }
    return sub[m][k];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, k;
    cin >> n >> m >> k;
    cin >> (strA + 1) >> (strB + 1);
    cout << dp(n, m, k) << "\n";
    return 0;
}