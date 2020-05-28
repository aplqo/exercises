#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
typedef float real;
const unsigned int maxn = 17;
constexpr real inf = numeric_limits<real>::max() / 2;
#define lowbit(x) ((x) & -(x))

struct
{
    int x, y;
} chl[maxn + 1];
real dis[maxn + 1][maxn + 1];
real f[maxn][1 << maxn][4], cycle[1 << maxn];
unsigned int lg[1 << maxn], pcnt[1 << maxn];

unsigned int popcnt(unsigned int n)
{
    unsigned int ret = 0;
    while (n)
    {
        ++ret;
        n -= lowbit(n);
    }
    return ret;
}
void init(const unsigned int n)
{
    for (unsigned int i = 0; i <= n; ++i)
        for (unsigned int j = i + 1; j <= n; ++j)
            dis[i][j] = dis[j][i] = hypot(chl[i].x - chl[j].x, chl[i].y - chl[j].y);
    for (unsigned int i = 0; i <= n; ++i)
        lg[1 << i] = i;
    for (unsigned int i = 0; i < (1 << n); ++i)
        for (unsigned int j = i; j; j &= j - 1)
        {
            const unsigned int p = lg[lowbit(j)];
            fill(f[p][i], f[p][i] + 4, inf);
        }
    fill(cycle + 1, cycle + (1 << n), inf);
}
unsigned int dp(const unsigned int n, const real v)
{
    const unsigned int msk = (1 << n) - 1;
    for (unsigned int i = 0; i < (1 << n); ++i)
    {
        const unsigned int mc = min(3u, popcnt(i)) + 1;
        for (unsigned int j = i; j; j &= j - 1)
        {
            const unsigned int from = lg[lowbit(j)];
            for (unsigned int c = 1; c <= mc; ++c)
            {
                const real cv = v / (1 << c);
                cycle[i] = min(cycle[i], f[from][i][c - 1] + dis[from + 1][0] / cv);
                if (c == 3)
                    continue;
                for (unsigned int k = (~i) & msk; k; k &= k - 1)
                {
                    const unsigned int lb = lowbit(k), to = lg[lb];
                    f[to][i | lb][c] = min(f[to][i | lb][c], f[from][i][c - 1] + dis[from + 1][to + 1] / cv);
                }
            }
        }
        for (unsigned int j = (~i) & msk; j; j &= j - 1)
        {
            const unsigned int lb = lowbit(j), to = lg[lb];
            f[to][i | lb][0] = min(f[to][i | lb][0], cycle[i] + dis[0][to + 1] / v);
        }
    }
    return round(cycle[msk]);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, v;
    cin >> n;
    for_each(chl, chl + n + 1, [](auto& i) { cin >> i.x >> i.y; });
    cin >> v;
    init(n);
    cout << dp(n, v) << endl;
    return 0;
}