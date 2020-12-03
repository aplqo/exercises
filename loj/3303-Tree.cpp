#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 525010, maxl = 20;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int a[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}
unsigned long long dfs(const unsigned int x, const unsigned int dep)
{
    static unsigned int val[maxn + 10], acc[maxl + 10][1u << maxl];
    {
        const unsigned int tp = dep + a[x];
        for (unsigned int j = 0, msk = 1; j <= maxl; ++j, msk <<= 1)
            acc[j][tp & (msk - 1)] ^= msk;
    }
    unsigned long long ret = 0;
    val[x] = a[x];
    for (unsigned int j = 0, msk = 1; j <= maxl; ++j, msk <<= 1)
        val[x] ^= acc[j][dep & (msk - 1)];
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        ret += dfs(i->to, dep + 1);
        val[x] ^= val[i->to];
    }
    for (unsigned int j = 0, msk = 1; j <= maxl; ++j, msk <<= 1)
        val[x] ^= acc[j][dep & (msk - 1)];
    return ret + val[x];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int p;
        cin >> p;
        addEdge(p, i);
    }
    cout << dfs(1, 0) << "\n";
    return 0;
}