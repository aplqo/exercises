#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;
using num_t = long long;
constexpr num_t ninf = numeric_limits<num_t>::min();
const int maxn = 50;

struct vertex
{
    num_t val;
    bool typ; // true: mul false: add
} v[maxn * 2 + 10];
num_t f[maxn * 2 + 10][maxn * 2 + 10];

inline void dp(const unsigned int n)
{
    copy(v, v + n, v + n);
    for (unsigned int i = 0; i < n * 2; ++i)
        fill(f[i], f[i] + n * 2, ninf);
    for (unsigned int i = 0; i < n * 2; ++i)
        f[i][i] = v[i].val;

    for (unsigned int d = 1; d < n * 2; ++d)
        for (unsigned int l = 0; l <= n * 2 - d; ++l)
        {
            const unsigned int r = l + d;
            for (unsigned int k = l; k < r; ++k)
            {
                const num_t cur = v[k + 1].typ ? (f[l][k] * f[k + 1][r]) : (f[l][k] + f[k + 1][r]);
                f[l][r] = max(f[l][r], cur);
            }
        }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    num_t ans = ninf;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        char typ;
        cin >> typ >> v[i].val;
        v[i].typ = (typ == 'x');
    }
    dp(n);
    for (unsigned int i = 0; i < n; ++i)
        ans = max(ans, f[i][i + n - 1]);
    cout << ans << endl;
    return 0;
}