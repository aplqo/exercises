#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;

long long trans[maxn + 10];
unsigned int p[maxn + 10];

namespace FTree
{
#define lowbit(x) ((x) & -(x))
    unsigned int val[maxn + 10];

    inline void increase(unsigned int pos, const unsigned int n)
    {
        for (; pos <= n; pos += lowbit(pos))
            ++val[pos];
    }
    unsigned int prefixSum(unsigned int x)
    {
        unsigned int ret = 0;
        for (; x; x -= lowbit(x))
            ret += val[x];
        return ret;
    }

#undef lowbit
};

static void discretize(const unsigned int n)
{
    static long long tmp[maxn + 10];
    copy(trans, trans + 1 + n, tmp);
    sort(tmp, tmp + 1 + n);
    long long* cur = unique(tmp, tmp + n + 1);
    for (unsigned int i = 0; i <= n; ++i)
        p[i] = lower_bound(tmp, cur, trans[i]) - tmp + 1;
}
unsigned long long count(const unsigned int n)
{
    unsigned long long ret = 0;
    FTree::increase(p[0], n);
    for (unsigned int i = 1; i <= n; ++i)
    {
        ret += FTree::prefixSum(p[i] - 1);
        FTree::increase(p[i], n);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    long long m, acc = 0;
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
    {
        long long v;
        cin >> v;
        acc += v;
        trans[i] = acc - i * m;
    }
    discretize(n);
    cout << count(n) << "\n";
    return 0;
}