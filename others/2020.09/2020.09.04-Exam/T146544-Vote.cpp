#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <type_traits>
using namespace std;
const unsigned int maxn = 2000;
using real = long double;

real p[maxn + 10], pre[maxn + 1][maxn + 1], suf[maxn + 1][maxn + 1];

template <bool rev>
void dp(real res[maxn + 1][maxn + 1], const unsigned int n)
{
    static conditional_t<rev, minus<unsigned int>, plus<unsigned int>> nxt;
    res[0][0] = 1;
    unsigned int pos;
    if constexpr (rev)
        pos = n;
    else
        pos = 1;
    for (unsigned int i = 1; i <= n; ++i, pos = nxt(pos, 1))
    {
        res[i][0] = res[i - 1][0] * (1 - p[pos]);
        for (unsigned int j = 1; j <= i; ++j)
            res[i][j] = res[i - 1][j - 1] * p[pos] + res[i - 1][j] * (1 - p[pos]);
    }
}
real solve(const unsigned int k)
{
    real ret = 0;
    for (unsigned int i = 0; i <= k; ++i)
    {
        const unsigned int mj = min(i, k / 2);
        real v = 0;
        for (unsigned int j = 0; j <= mj; ++j)
            v += pre[i][j] * suf[k - i][k / 2 - j];
        ret = max(ret, v);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    copy_n(istream_iterator<real>(cin), n, p + 1);
    sort(p + 1, p + 1 + n);
    dp<false>(pre, n);
    dp<true>(suf, n);
    cout << fixed << solve(k) << "\n";
    return 0;
}