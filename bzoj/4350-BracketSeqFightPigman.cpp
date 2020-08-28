#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 300;
constexpr unsigned long long mod = 998244353;

unsigned int a[maxn + 1][maxn + 1];

static void initSum(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            a[i][j] += a[i - 1][j] + a[i][j - 1] - a[i - 1][j - 1];
}
inline bool test(const unsigned int x1, const unsigned int y1, const unsigned int x2, const unsigned int y2)
{
    return !(a[x2][y2] + a[x1 - 1][y1 - 1] - a[x2][y1 - 1] - a[x1 - 1][y2]);
}
bool testSolution(const unsigned int n)
{
    static bool con[maxn + 1][maxn + 1];
    for (unsigned int i = 0; i <= n; ++i)
        copy(a[i], a[i] + 1 + n, con[i]);
    for (unsigned int k = 0; k <= n; ++k)
        for (unsigned int l = 0; l <= n; ++l)
            for (unsigned int r = 0; r <= n; ++r)
                con[l][r] = con[l][r] || (con[l][r] && con[k][r]);
    for (unsigned int i = 0; i <= n; ++i)
        if (con[i][i])
            return false;
    return true;
}
unsigned long long dp(const unsigned int n)
{
    static unsigned long long f[maxn + 1][maxn + 1];
    memset(f, 0, sizeof(f));
    for (unsigned int i = 1; i <= n; ++i)
        f[i][i] = 1;
    for (unsigned int i = 2; i <= n; ++i)
        for (unsigned int l = 1; l + i - 1 <= n; ++l)
        {
            const unsigned int r = l + i - 1;
            for (unsigned int k = l + 1; k < r; ++k)
                if (test(l, l + 1, l, k) && test(k + 1, l, r, k))
                    f[l][r] = (f[l][r] + f[l + 1][k] * f[k + 1][r] % mod) % mod;
            if (test(l, l + 1, l, r))
                f[l][r] = (f[l][r] + f[l + 1][r]) % mod;
            if (test(l + 1, l, r, l))
                f[l][r] = (f[l][r] + f[l + 1][r]) % mod;
        }
    return f[1][n];
}
void solve()
{
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        a[u][v] = 1;
    }
    if (testSolution(n))
    {
        initSum(n);
        cout << dp(n) << "\n";
    }
    else
        cout << "0\n";
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
        solve();
        memset(a, 0, sizeof(a));
    }
    return 0;
}
