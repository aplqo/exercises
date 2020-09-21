#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1000, maxm = 1000;
constexpr unsigned long long mod = 1e7 + 7;

int cost[maxn + 10];

unsigned long long count(const unsigned int n, const int m)
{
    static int sum[maxn + 10];
    static unsigned long long f[maxm + 10] = { 1 };
    sort(cost + 1, cost + n + 1, greater<int>());
    partial_sum(cost + 1, cost + 1 + n, sum + 1);
    if (sum[n] <= m)
        return 1;
    unsigned long long ret = 0;
    for (unsigned int i = 1; i <= n + 1; ++i)
    {
        {
            const int s = sum[n] - sum[i];
            for (int j = max(0, m + 1 - cost[i] - s); j <= m - s; ++j)
                ret = (ret + f[j]) % mod;
        }
        for (int j = m; j >= cost[i]; --j)
            f[j] = (f[j] + f[j - cost[i]]) % mod;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<unsigned int>(cin), n, cost + 1);
    cout << count(n, m) << "\n";
    return 0;
}