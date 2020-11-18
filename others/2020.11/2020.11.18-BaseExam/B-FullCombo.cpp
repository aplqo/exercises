#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 1000000;

unsigned long long a[maxn + 10];
unsigned int t[maxn + 10];
vector<unsigned int> upd[maxn + 10];
namespace FTree
{
#define lowbit(x) ((x) & -(x))
    unsigned long long val[maxn + 10];

    void update(unsigned int pos, const unsigned int n, const unsigned long long nv)
    {
        for (; pos <= n; pos += lowbit(pos))
            val[pos] = max(val[pos], nv);
    }
    unsigned long long prefixMax(unsigned int pos)
    {
        unsigned long long ret = 0;
        for (; pos; pos -= lowbit(pos))
            ret = max(ret, val[pos]);
        return ret;
    }

#undef lowbit
}

unsigned long long dp(const unsigned int n)
{
    static unsigned long long f[maxn + 19];
    for (unsigned int i = 1; i <= n; ++i)
        upd[t[i] >= i ? 0 : i - t[i]].push_back(i);
    for (unsigned int i = 1; i <= n; ++i)
    {
        f[i] += a[i];
        FTree::update(min(i + t[i], n + 1), n + 1, f[i]);
        for (unsigned int j : upd[i])
            f[j] = FTree::prefixMax(j);
    }
    return *max_element(f, f + n + 1);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, t + 1);
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned long long tp;
        cin >> tp;
        a[i] = tp * t[i];
    }
    cout << dp(n) << "\n";
    return 0;
}