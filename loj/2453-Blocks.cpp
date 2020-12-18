#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6;

long long a[maxn + 10], sum[maxn + 10];

unsigned int maxSeq(const unsigned int n, const long long k)
{
    static unsigned int stk[maxn + 10];
    unsigned int ret = 0, *top = stk;
    *(top++) = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        sum[i] = sum[i - 1] + a[i] - k;
        if (sum[i] < sum[*(top - 1)])
            *(top++) = i;
    }
    for (unsigned int i = n; i; --i)
    {
        while (top > stk && sum[i] >= sum[*(top - 1)])
            --top;
        ret = max(ret, i - *top);
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
    copy_n(istream_iterator<long long>(cin), n, a + 1);
    for (unsigned int i = 0; i < m; ++i)
    {
        long long k;
        cin >> k;
        cout << maxSeq(n, k) << " ";
    }
    cout.put('\n');
    return 0;
}