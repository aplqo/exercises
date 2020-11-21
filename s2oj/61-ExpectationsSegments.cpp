#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000, maxl = 32;

unsigned int a[maxn + 10];

unsigned long long sumAnd(const unsigned int n)
{
    unsigned long long ret = 0;
    unsigned int lst = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int cb = a[i] & 0x01;
        if (!cb)
            lst = i;
        ret += i - lst;
    }
    return ret;
}
unsigned long long sumOr(const unsigned int n)
{
    unsigned long long ret = 0;
    unsigned int lst = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int cb = a[i] & 0x01;
        if (cb)
            lst = i;
        ret += lst;
    }
    return ret;
}
unsigned long long sumXor(const unsigned int n)
{
    unsigned long long ret = 0;
    unsigned int lst[2] {};
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int cb = a[i] & 0x01;
        if (cb)
            swap(lst[0], lst[1]);
        ++lst[cb];
        ret += lst[1];
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
    unsigned long long sa = 0, so = 0, sx = 0;
    const unsigned long long sum = accumulate(a, a + 1 + n, 0ull);
    for (unsigned int i = 0; i < maxl; ++i)
    {
        const unsigned long long rat = 1u << i;
        sa += sumAnd(n) * rat;
        so += sumOr(n) * rat;
        sx += sumXor(n) * rat;
        for_each(a + 1, a + 1 + n, [](unsigned int& i) { i >>= 1; });
    }
    cout.precision(3);
    const double cn = n * static_cast<double>(n);
    const auto getSum = [sum, cn](unsigned long long v) { return ((v - sum) * 2 + sum) / cn; };
    cout << fixed << getSum(sx) << " " << getSum(sa) << " " << getSum(so) << "\n";
    return 0;
}