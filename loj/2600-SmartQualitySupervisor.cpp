#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 200000, maxm = 200000;

struct Range
{
    unsigned int l, r;
} rng[maxm + 1];
unsigned long long w[maxn + 1], v[maxn + 1];
unsigned int n, m;

static void discretize(const unsigned int n)
{
    static unsigned long long tmp[maxn + 1];
    copy(w + 1, w + 1 + n, tmp);
    sort(tmp, tmp + n);
    unsigned long long* const e = unique(tmp, tmp + n);
    for (unsigned long long* i = w + 1; i < w + 1 + n; ++i)
        *i = lower_bound(tmp, e, *i) - tmp;
}
long long testVal(const unsigned long long W)
{
    static unsigned long long cntSum[maxn + 1], vSum[maxn + 1];
    for (unsigned int i = 1; i <= n; ++i)
    {
        cntSum[i] = cntSum[i - 1];
        vSum[i] = vSum[i - 1];
        if (w[i] >= W)
        {
            ++cntSum[i];
            vSum[i] += v[i];
        }
    }
    unsigned long long ret = 0;
    for (const Range* i = rng; i < rng + m; ++i)
        ret += (cntSum[i->r] - cntSum[i->l - 1]) * (vSum[i->r] - vSum[i->l - 1]);
    return ret;
}
unsigned long long binaryFind(const unsigned int n, const long long s)
{
    unsigned int l = 0, r = n;
    while (l <= r)
    {
        const unsigned int m = (l + r) >> 1;
        if (testVal(m) > s)
            l = m + 1;
        else
            r = m - 1;
    }
    return min(abs(testVal(l) - s), abs(testVal(r) - s));
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long s;
    cin >> n >> m >> s;
    for (unsigned int i = 1; i <= n; ++i)
        cin >> w[i] >> v[i];
    for (Range* i = rng; i < rng + m; ++i)
        cin >> i->l >> i->r;
    discretize(n);
    cout << binaryFind(n, s) << "\n";
    return 0;
}