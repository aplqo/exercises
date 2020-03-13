#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num_t = unsigned long long;
const int maxn = 10, maxc = 890, maxs = (1 << maxn) - 1;

unsigned int cnt[maxc + 10], st[maxc + 10], scur;
num_t f[maxc + 10][maxn + 10][maxn * maxn + 10];

inline unsigned int popcount(unsigned int n)
{
    unsigned int ret = 0;
    for (unsigned int t = 1; n; t <<= 1)
        if (n & t)
        {
            ++ret;
            n ^= t;
        }
    return ret;
}
inline void GetVavid(const unsigned int n)
{
    scur = 0;
    for (unsigned int i = 0; i < (1 << n); ++i)
    {
        if (i & (i << 1) || i & (i >> 1))
            continue;
        st[scur] = i;
        cnt[scur] = popcount(i);
        f[scur][0][cnt[scur]] = 1;
        ++scur;
    }
}
inline num_t dp(const unsigned int n, const unsigned int k)
{
    for (unsigned int i = 0; i < n; ++i)
        f[0][i][0] = 1;
    for (unsigned int i = 1; i <= k; ++i)
        for (unsigned int j = 1; j < n; ++j)
            for (unsigned int t = 0; t < scur; ++t)
            {
                if (cnt[t] > i)
                    continue;
                for (unsigned int v = 0; v < scur; ++v)
                {
                    if (st[t] & st[v] || st[t] & (st[v] << 1) || st[t] & (st[v] >> 1))
                        continue;
                    f[t][j][i] += f[v][j - 1][i - cnt[t]];
                }
            }
    num_t ans = 0;
    for (unsigned int i = 0; i < scur; ++i)
        ans += f[i][n - 1][k];
    return ans;
}
int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, k;
    cin >> n >> k;
    GetVavid(n);
    cout << dp(n, k) << endl;
    return 0;
}