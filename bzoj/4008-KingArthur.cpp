#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
using namespace std;
const unsigned int maxn = 220, maxr = 132;
typedef double real;

unsigned int d[maxn + 1];
real np[maxn + 1][maxr + 1], f[maxn + 1][maxn + 1];

static void init(const unsigned int n, const unsigned int r)
{
    for (unsigned int i = 0; i < n; ++i)
        np[i][0] = 1;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 2; j <= r; ++j)
            np[i][j] = np[i][j - 1] * np[i][1];
}
void dp(const unsigned int n, const unsigned int r)
{
    f[0][0] = np[0][r];
    f[0][1] = 1 - f[0][0];
    for (unsigned int i = 1; i < n; ++i)
    {
        f[i][0] = f[i - 1][0] * np[i][r];
        const unsigned int li = min(i + 1, r);
        for (unsigned int j = 1; j <= li; ++j)
            f[i][j] = f[i - 1][j] * np[i][r - j] + f[i - 1][j - 1] * (1 - np[i][r + 1 - j]);
    }
}
real sum(const unsigned int n, const unsigned int r)
{
    real ret = d[0] * (1 - np[0][r]);
    for (unsigned int i = 1; i < n; ++i)
    {
        real tp = 0;
        const unsigned int li = min(i, r);
        for (unsigned int j = 0; j <= li; ++j)
            tp += f[i - 1][j] * (1 - np[i][r - j]);
        ret += tp * d[i];
    }
    return ret;
}
real solve()
{
    unsigned int n, r;
    cin >> n >> r;
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> np[i][1] >> d[i];
        np[i][1] = 1 - np[i][1];
    }
    init(n, r);
    dp(n, r);
    return sum(n, r);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        printf("%.10lf\n", solve());
    return 0;
}