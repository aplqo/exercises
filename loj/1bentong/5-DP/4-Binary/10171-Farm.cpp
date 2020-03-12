#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
using num_t = unsigned int;
const int maxn = 12, maxs = 1 << maxn;
constexpr num_t mod = 1e8;

unsigned int gnd[maxn + 10];
num_t f[maxn + 10][maxs + 10];

#define invavid(n, i) ((gnd[n] & (i)) ^ (i) || (i) & ((i) << 1) || (i) & ((i) >> 1))
inline num_t dp(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 0; i < (1 << n); ++i)
        f[0][i] = invavid(0, i) ? 0 : 1;
    for (unsigned int i = 1; i < m; ++i)
        for (unsigned int j = 0; j < (1 << n); ++j)
        {
            if (invavid(i, j))
                continue;
            for (unsigned int lst = 0; lst < (1 << n); ++lst)
            {
                if (invavid(i - 1, lst) || j & lst)
                    continue;
                f[i][j] = (f[i][j] + f[i - 1][lst]) % mod;
            }
        }
    return accumulate(f[m - 1], f[m - 1] + (1 << n), num_t(0), [](const num_t v, const num_t cur) { return (v + cur) % mod; });
}
int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, m;
    cin >> m >> n;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int t = 1;
        for (unsigned int j = 0; j < n; ++j, t <<= 1)
        {
            unsigned int tmp;
            cin >> tmp;
            if (tmp)
                gnd[i] |= t;
        }
    }
    cout << dp(n, m) << endl;
    return 0;
}