/*Luogu team T132588: Paladin Waterfall*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdio>
using namespace std;
using real = double;
const char fmt[] = "%.7lf\n";
const unsigned int maxn = 2000;

real f[maxn + 10][maxn + 10];

real dp(const unsigned int n, const real p, const unsigned int t)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= t; ++j)
            f[i][j] = p * (f[i - 1][j - 1] + 1) + (1 - p) * f[i][j - 1];
    return round(f[n][t] * 1e7) / 1e7;
}
int main()
{
    unsigned int n, t;
    double p;
    scanf("%u%lf%u", &n, &p, &t);
    printf(fmt, dp(n, p, t));
    return 0;
}
