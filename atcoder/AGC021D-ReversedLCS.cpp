#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 300;

char str[maxn + 10];

unsigned int dp(const unsigned int n, const unsigned int k)
{
    static unsigned int f[maxn + 10][maxn + 10][maxn + 10];
    for (unsigned int i = 0; i < n; ++i)
        fill(f[i][i], f[i][i] + k + 1, 1);
    for (unsigned int i = 0; i + 1 < n; ++i)
    {
        fill(f[i][i + 1] + 1, f[i][i + 1] + k + 1, 2);
        f[i][i + 1][0] = str[i] == str[i + 1] ? 2 : 1;
    }
    for (unsigned int i = 3; i <= n; ++i)
        for (unsigned int l = 0; l + i <= n; ++l)
        {
            const unsigned int r = l + i - 1;
            if (str[l] == str[r])
            {
                f[l][r][0] = max({ f[l + 1][r - 1][0] + 2, f[l][r - 1][0], f[l + 1][r][0] });
                for (unsigned int t = 1; t <= k; ++t)
                    f[l][r][t] = max({ f[l][r][t - 1],
                        f[l + 1][r - 1][t] + 2,
                        f[l][r - 1][t], f[l + 1][r][t] });
            }
            else
            {
                f[l][r][0] = max({ f[l + 1][r - 1][0], f[l][r - 1][0], f[l + 1][r][0] });
                for (unsigned int t = 1; t <= k; ++t)
                    f[l][r][t] = max({ f[l][r][t - 1],
                        f[l + 1][r - 1][t], f[l + 1][r - 1][t - 1] + 2,
                        f[l][r - 1][t], f[l + 1][r][t] });
            }
        }
    return f[0][n - 1][k];
}

int main()
{
#ifndef APTEST
    ios::sync_with_stdio(false);
#endif
    unsigned int k;
    cin >> str >> k;
    cout << dp(strlen(str), k) << "\n";
    return 0;
}