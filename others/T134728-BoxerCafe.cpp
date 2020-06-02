#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const int maxn = 100000, maxm = 100000;

long long h[maxm + 2], p[maxn + 2];
long long* mv[maxn + 2];

void init(const unsigned int n, const unsigned int m)
{
    long long* lst = p + 1;
    for (unsigned int i = 0; i <= n; ++i)
        lst = mv[i] = lower_bound(lst, p + m + 1, h[i]);
}
bool test(const unsigned int n, const unsigned int m, const long long val)
{
    long long* ptr = p + 1;
    const auto valid = [&m, &val](long long* const ptr, long long x) -> long long* {
        return ptr >= p + m + 1 || x - *(ptr - 1) <= val ? ptr : nullptr;
    };
    for (unsigned int i = 0; i < n && ptr < p + m + 1; ++i)
    {
        if (h[i] > *ptr + val)
            return false;
        if (h[i] >= *ptr)
        {
            const long long v = max(val + 2 * (*ptr) - h[i], (val + h[i] + *ptr) / 2);
            if (v < h[i])
                return false;
            long long* const p0 = valid(upper_bound(ptr, mv[i + 1], v), h[i]);
            if (!p0)
                return false;
            ptr = max(ptr, p0);
        }
        else
        {
            long long* const p0 = upper_bound(ptr, mv[i + 1], h[i] + val);
            ptr = max(ptr, p0);
        }
    }
    return ptr >= p + m + 1;
}
unsigned long long solve(const unsigned int n, const unsigned int m)
{
    long long l = 0, r = min(abs(h[0] - p[1]), abs(h[0] - p[m])) + p[m] - p[1];
    while (l <= r)
    {
        long long mid = (l + r) >> 1;
        if (test(n, m, mid))
            r = mid - 1;
        else
            l = mid + 1;
    }
    return l;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<long long>(cin), n, h);
    copy_n(istream_iterator<long long>(cin), m, p + 1);
    h[n] = p[m] + 1;
    init(n, m);
    cout << solve(n, m) << endl;
    return 0;
}