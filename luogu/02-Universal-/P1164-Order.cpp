#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <iostream>
#include <algorithm>
#include <numeric>
using namespace std;
const int maxn = 105, maxm = 10005;

int dat[maxn], ans[maxm * maxn][maxn], sum[maxn];

int main()
{
    int n, m;
    cin >> n >> m;
    for (int *i = dat + 1; i < dat + 1 + n; ++i)
        cin >> *i;
    sort(dat + 1, dat + 1 + n);
    partial_sum(dat + 1, dat + 1 + n, sum + 1);
    for (int i = 1; i <= n; ++i)
        ans[dat[i]][i] = 1;
    for (int oi = 1; oi <= n; ++oi)
        for (int mi = 1; mi <= sum[oi]; ++mi)
        {
            ans[mi][oi] += ans[mi][oi - 1];
            if (mi >= dat[oi])
                ans[mi][oi] += ans[mi - dat[oi]][oi - 1];
        }
    cout << ans[m][n];
    return 0;
}
