#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100, maxk = 10000;
typedef int Array[maxn + 10][maxn + 10][maxn * maxn / 4 + 10];

int a[maxn + 10];

template <int step>
void dp(const int outBegin, const int outEnd, const int inBegin, const int inEnd, const int n, Array res)
{
    for (int in = inBegin; in != inEnd; in += step)
        for (int out = outBegin; out != outEnd; out += step)
            for (int sp = 0; sp <= n * n / 4; ++sp)
            {
                res[in][out][sp] = max(res[in - step][out][sp], res[in][out - step][sp]);
                if (sp >= abs(in - out))
                    res[in][out][sp] = max(res[in][out][sp], res[in - step][out - step][sp - abs(in - out)] + a[in] - a[out]);
            }
}
unsigned int dp(const unsigned int n, const unsigned int l, const unsigned int r, const unsigned int k)
{
    static Array dl, dr;
    dp<1>(1, l, l, r + 1, n, dl);
    dp<(-1)>(n, r, r, l - 1, n, dr);
    int ret = 0;
    const unsigned int mpk = min(n * n / 4, k);
    for (unsigned int i = l - 1; i <= r; ++i)
        for (unsigned int j = 0; j <= mpk; ++j)
            ret = max(ret, dl[i][l - 1][j] + dr[i + 1][r + 1][mpk - j]);
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, l, r, k;
    cin >> n >> l >> r >> k;
    copy_n(istream_iterator<int>(cin), n, a + 1);
    cout << accumulate(a + l, a + r + 1, 0u) - dp(n, l, r, k) << "\n";
    return 0;
}