#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
using namespace std;
using res_t = uint_fast16_t;
#define lowbit(x) ((x) & -(x))
const int maxn = 16;
constexpr res_t maxs = (res_t(1) << maxn);
constexpr int inf = INT_MAX;

int dis[maxn + 10][maxn + 10], f[maxs + 10];
unsigned int lg[maxs + 10];

inline void init(const unsigned int n)
{
    res_t t = 1;
    for (unsigned int i = 0; i < n; ++i, t <<= 1)
    {
        lg[t] = i;
        f[t] = 0;
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
            cin >> dis[i][j];
    fill(f + 1, f + (res_t(1) << n), inf);
    init(n);

    for (res_t i = 1; i < res_t(1) << n; ++i)
    {
        for (res_t t = i; t; t -= lowbit(t))
        {
            const unsigned int p = lg[lowbit(t)];
            for (res_t k = i; k; k -= lowbit(k))
                if (lg[lowbit(k)] != p)
                    f[i] = min(f[i], f[i ^ lowbit(t)] + dis[lg[lowbit(k)]][p]);
        }
    }
    cout << f[(res_t(1) << n) - 1] << endl;
    return 0;
}
