#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 2000;
typedef double real;

real dp(const unsigned int n, const unsigned int t, const real p)
{
    static real f[maxn + 10][maxn + 10];
    for (unsigned int i = 1; i <= t; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            f[i][j] = f[i - 1][j] * (1 - p) + p * (f[i - 1][j - 1] + 1);
    return f[t][n];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    real p;
    unsigned int n, t;
    cin >> n >> p >> t;
    cout.precision(7);
    cout << fixed << (round(dp(n, t, p) * 1e7) / 1e7) << "\n";
    return 0;
}