#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 5e6;

long long sum[maxn + 10];

long long dp(const unsigned int n, const unsigned int k)
{
    static long long f[maxn + 10], pre[maxn + 10];
    long long mx = 0;
    for (unsigned int i = k; i <= n; ++i)
    {
        mx = max(mx, pre[i - k] - sum[i - k]);
        f[i] = mx + sum[i];
        pre[i] = max(pre[i - 1], f[i]);
    }
    return pre[n];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    partial_sum(istream_iterator<long long>(cin), istream_iterator<long long>(), sum + 1);
    cout << dp(n, k) << "\n";
    return 0;
}