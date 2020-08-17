#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 2000, maxl = 42;
constexpr unsigned long long inf = (1ull << maxl) - 1;

unsigned long long sum[maxn + 1];

unsigned long long dpRange(const unsigned int n, const unsigned int a, const unsigned int b)
{
    static bool f[maxn + 1][maxn + 1];
    unsigned long long ret = 0;
    for (unsigned int i = maxl; i > 0; --i)
    {
        const unsigned long long bmsk = 1ull << (i - 1), lmsk = inf - (bmsk - 1);
        memset(f, 0, sizeof(f));
        f[0][0] = true;
        for (unsigned int i = 1; i <= n; ++i)
        {
            const unsigned int mj = min(i, b);
            for (unsigned int j = 1; j <= mj; ++j)
                for (unsigned int k = 0; k < i && !f[i][j]; ++k)
                    f[i][j] = f[k][j - 1] && !((sum[i] - sum[k]) & bmsk)
                        && (((sum[i] - sum[k]) & lmsk) | ret) == ret;
        }
        if (none_of(f[n] + a, f[n] + b + 1, [](bool v) { return v; }))
            ret |= bmsk;
    }
    return ret;
}
unsigned long long dpLimit(const unsigned int n, const unsigned int b)
{
    static unsigned int f[maxn + 1];
    unsigned long long ret = 0;
    for (unsigned int i = maxl; i > 0; --i)
    {
        const unsigned long long bmsk = 1ull << (i - 1), lmsk = inf - (bmsk - 1);
        fill(f + 1, f + n + 1, n + 10);
        for (unsigned int i = 1; i <= n; ++i)
            for (unsigned int k = 0; k < i; ++k)
                if (!((sum[i] - sum[k]) & bmsk) && (((sum[i] - sum[k]) & lmsk) | ret) == ret)
                    f[i] = min(f[i], f[k] + 1);
        if (f[n] > b)
            ret |= bmsk;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, a, b;
    cin >> n >> a >> b;
    copy_n(istream_iterator<unsigned long long>(cin), n, sum + 1);
    partial_sum(sum + 1, sum + 1 + n, sum + 1);
    if (a != 1)
        cout << dpRange(n, a, b) << endl;
    else
        cout << dpLimit(n, b) << endl;
    return 0;
}