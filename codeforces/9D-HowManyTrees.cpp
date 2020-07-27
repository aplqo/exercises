#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 35;

long long dp(const unsigned int n, const unsigned int h)
{
    static long long f[maxn + 1][maxn + 1];
    fill(f[0], f[0] + 1 + n, 1);
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            for (unsigned int k = 0; k < i; ++k)
                f[i][j] += f[k][j - 1] * f[i - 1 - k][j - 1];
    return f[n][n] - f[n][h - 1];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, h;
    cin >> n >> h;
    cout << dp(n, h) << endl;
    return 0;
}