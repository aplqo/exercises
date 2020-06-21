#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxl = 10, maxd = 1000;
#define lowbit(x) ((x) & -(x))

unsigned long long f[1 << maxl][maxd];
unsigned int buf[maxl + 1], cnt[10];
unsigned int lg[(1 << maxl) + 1], fac[maxl + 1] = { 1 };

void getLog(const unsigned int n)
{
    for (unsigned int i = 0; i <= n; ++i)
        lg[1 << i] = i;
}
void getFac(unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        fac[i] = fac[i - 1] * i;
}
unsigned long long dp(const unsigned int l, const unsigned int d)
{
    const unsigned int msk = (1 << l) - 1;
    for (unsigned int i = 0; i < l; ++i)
        f[1 << i][buf[i] % d] = 1;
    for (unsigned int i = 1; i <= msk; ++i)
    {
        for (unsigned int r = 0; r < d; ++r)
        {
            if (!f[i][r])
                continue;
            for (unsigned int j = (~i) & msk; j; j -= lowbit(j))
            {
                const unsigned int b = lowbit(j);
                const unsigned int t = buf[lg[b]];
                f[i | b][(r * 10 + t) % d] += f[i][r];
            }
        }
    }
    unsigned long long ret = f[msk][0];
    for (unsigned int i = 0; i < 10; ++i)
        ret /= fac[cnt[i]];
    return ret;
}
void solve()
{
    unsigned int d;
    char b[maxl + 2];
    cin >> b >> d;
    const unsigned int l = strlen(b);
    for (unsigned int i = 0; i < l; ++i)
    {
        buf[i] = b[i] - '0';
        ++cnt[buf[i]];
    }
    cout << dp(l, d) << endl;
    memset(f, 0, sizeof(f));
    memset(cnt, 0, sizeof(cnt));
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    getLog(maxl);
    getFac(maxl);
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}