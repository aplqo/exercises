#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
using namespace std;
typedef long long num_t;
const int maxn = 1e5;

num_t f[maxn + 10] = { 0, 1 }, mod;
num_t sum[maxn + 10][2] = { { 0, 0 }, { 1, 1 }, { 2, 1 } };

int main()
{
#if (!defined(APTEST)) && (!defined(APDEBUG))
    freopen("running.in", "r", stdin);
    freopen("running.out", "w", stdout);
#endif
    unsigned int n;
    scanf("%u%lld", &n, &mod);
    for (unsigned int i = 2; i <= n; ++i)
    {
        const unsigned int t = i / 2, pos = i + 1;
        f[i] = (sum[pos - 2 * t][1] % mod + mod) % mod;
        f[i] = ((f[i] + sum[t + (i % 2)][0]) % mod + mod) % mod;
        sum[pos][0] = (sum[pos - 1][0] + f[i]) % mod;
        sum[pos - 2][1] = (sum[pos - 2][1] + f[i]) % mod;
    }
    f[n] = f[n] % static_cast<num_t>(1e9 + 7);
    printf("%lld\n", f[n]);
    return 0;
}
