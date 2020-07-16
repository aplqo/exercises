#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 16, maxs = (1 << maxn);
#define lowbit(x) ((x) & (-x))

unsigned int w[maxn + 1], t[maxn + 1];
unsigned int sumW[maxs + 1], maxT[maxs + 1], lg[maxs + 1];

static void init(const unsigned int n)
{
    const unsigned int msk = (1 << n) - 1;
    for (unsigned int i = 0; i <= n; ++i)
        lg[1 << i] = i;
    for (unsigned int i = 1; i <= msk; ++i)
    {
        const unsigned int p = lowbit(i);
        sumW[i] = sumW[i ^ p] + w[lg[p]];
        maxT[i] = max(maxT[i ^ p], t[lg[p]]);
    }
}
unsigned int dp(const unsigned int n, const unsigned int mw)
{
    static unsigned int f[maxs + 1];
    const unsigned int msk = (1 << n) - 1;
    fill(f, f + msk + 1, UINT_MAX / 2);
    for (unsigned int i = 1; i <= msk; ++i)
    {
        for (unsigned int j = i; j; j = (j - 1) & i)
            if (sumW[i ^ j] <= mw)
                f[i] = min(f[i], f[j] + maxT[i ^ j]);
        if (sumW[i] <= mw)
            f[i] = min(f[i], maxT[i]);
    }
    return f[msk];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, mw;
    cin >> mw >> n;
    for (unsigned int i = 0; i < n; ++i)
        cin >> t[i] >> w[i];
    init(n);
    cout << dp(n, mw) << endl;
    return 0;
}