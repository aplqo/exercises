#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 400;
constexpr unsigned int inf = UINT_MAX / 2;

unsigned int a[maxn + 10], b[maxn + 10];
unsigned int t[maxn + 10];

unsigned int dp(const unsigned int n)
{
    static unsigned int f[maxn + 10][maxn + 10];
    fill(f[0], f[0] + 1 + n, inf);
    for (unsigned int i = 1; i <= n; ++i)
    {
        fill(f[i], f[i] + 1 + n, inf);
        f[i][i] = t[i] * (b[i - 1] + b[i + 1]);
    }
    for (unsigned int i = 2; i <= n; ++i)
        for (unsigned int l = 1; l + i <= n + 1; ++l)
        {
            const unsigned int r = l + i - 1, sb = b[l - 1] + b[r + 1];
            for (unsigned int k = l + 1; k < r; ++k)
                f[l][r] = min(f[l][k - 1] + f[k + 1][r] + t[k] * sb, f[l][r]);
            f[l][r] = min({ f[l][r], sb * t[l] + f[l + 1][r], sb * t[r] + f[l][r - 1] });
        }
    return f[1][n];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, atk, acc = 0;
    cin >> n >> atk;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int h;
        cin >> a[i] >> b[i] >> h;
        t[i] = (h + atk - 1) / atk;
        acc += a[i] * t[i];
    }
    cout << dp(n) + acc << "\n";
    return 0;
}