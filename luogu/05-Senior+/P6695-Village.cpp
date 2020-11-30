#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int n;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
unsigned int dfsCentroid(const unsigned int x, const unsigned int f, unsigned int& pos, unsigned int& mxs)
{
    unsigned int ret = 1, ms = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        const unsigned int sz = dfsCentroid(to, x, pos, mxs);
        ret += sz;
        ms = max(ms, sz);
    }
    ms = max(ms, n - ret);
    if (ms < mxs)
    {
        mxs = ms;
        pos = x;
    }
    return ret;
}
unsigned int treeCentroid(const unsigned int root, const unsigned int n)
{
    unsigned int ret, ms = UINT_MAX;
    dfsCentroid(root, 0, ret, ms);
    return ret;
}
namespace MinDist
{
    unsigned int seq[maxn + 10];

    template <bool isRoot>
    unsigned int dfs(const unsigned int x, const unsigned int f)
    {
        unsigned int ret = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
            if (i->to != f)
                ret += dfs<false>(i->to, x);
        if (seq[x] == x)
        {
            ret += 2;
            if constexpr (isRoot)
                swap(seq[x], seq[head[x]->to]);
            else
                swap(seq[x], seq[f]);
        }
        return ret;
    }
    static unsigned int solve(const unsigned int n)
    {
        iota(seq + 1, seq + 1 + n, 1);
        return dfs<true>(1, 0);
    }
}
namespace MaxDist
{
    unsigned int dfn[maxn + 10], rev[maxn + 10], siz[maxn + 10];
    unsigned int seq[maxn + 10];

    unsigned long long dfs(const unsigned int x, const unsigned int f)
    {
        static unsigned int sta = 0;
        dfn[x] = sta++;
        rev[dfn[x]] = x;
        siz[x] = 1;
        unsigned long long ret = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (to == f)
                continue;
            ret += dfs(to, x) + 2 * min(n - siz[to], siz[to]);
            siz[x] += siz[to];
        }
        return ret;
    }
    unsigned int maxSubtree(const unsigned int x)
    {
        unsigned int ret = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
            ret = max(ret, siz[i->to]);
        return ret;
    }
    unsigned long long solve(const unsigned int n)
    {
        const unsigned int rt = treeCentroid(1, n);
        const unsigned long long ret = dfs(rt, 0);
        const unsigned int ms = maxSubtree(rt);
        for (unsigned int i = 1; i <= n; ++i)
            seq[i] = rev[(dfn[i] + ms) % n];
        return ret;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    cout << MinDist::solve(n) << " " << MaxDist::solve(n) << "\n";
    copy(MinDist::seq + 1, MinDist::seq + 1 + n, ostream_iterator<unsigned int>(cout, " "));
    cout.put('\n');
    copy(MaxDist::seq + 1, MaxDist::seq + 1 + n, ostream_iterator<unsigned int>(cout, " "));
    cout.put('\n');
    return 0;
}