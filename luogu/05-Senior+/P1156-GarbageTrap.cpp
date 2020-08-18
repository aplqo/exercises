#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 100, maxd = 100;

struct Obj
{
    unsigned int t, F, h;
} o[maxn + 10];

unsigned int dp(const unsigned int n, const unsigned int d)
{
    static unsigned int f[maxn + 10][maxd + 10];
    f[0][0] = 10;
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 0; j <= d; ++j)
        {
            const Obj& ob = o[i];
            if (j >= ob.h && f[i - 1][j - ob.h] >= ob.t)
                f[i][j] = max(f[i][j], f[i - 1][j - ob.h]);
            if (f[i - 1][j] >= ob.t)
                f[i][j] = max(f[i][j], f[i - 1][j] + ob.F);
        }
    unsigned int ret = UINT_MAX;
    for (unsigned int i = 0; i <= n; ++i)
        if (f[i][d])
            ret = min(ret, f[i][d]);
    if (ret == UINT_MAX)
    {
        ret = 0;
        for (unsigned int i = 0; i <= n; ++i)
            ret = max(ret, *max_element(f[i], f[i] + d + 1));
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int d, g;
    cin >> d >> g;
    for (Obj* i = o + 1; i < o + 1 + g; ++i)
        cin >> i->t >> i->F >> i->h;
    sort(o + 1, o + 1 + g, [](const Obj& a, const Obj& b) { return a.t < b.t; });
    cout << dp(g, d) << endl;
    return 0;
}