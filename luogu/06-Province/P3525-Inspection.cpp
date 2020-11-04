#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000000;

struct Edge
{
    unsigned int to;
    const Edge* pre;
};
const Edge* head[maxn + 10];
unsigned int siz[maxn + 10], mdep[maxn + 10];
unsigned int centroid[2], *ccur = centroid;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge ed[maxn * 2 + 10], *cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
void dfsCentroid(const unsigned int x, const unsigned int f, const unsigned int n)
{
    static unsigned int csiz = UINT_MAX;
    siz[x] = 1;
    unsigned int ms = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dfsCentroid(to, x, n);
        ms = max(ms, siz[to]);
        siz[x] += siz[to];
    }
    ms = max(ms, n - siz[x]);
    if (ms < csiz)
    {
        csiz = ms;
        ccur = centroid + 1;
        centroid[0] = x;
    }
    else if (ms == csiz)
        *(ccur++) = x;
}
template <bool isRoot>
pair<unsigned long long, unsigned int> dfsChain(const unsigned int x, const unsigned int f, const unsigned int dep)
{
    pair<unsigned long long, unsigned int> ret(dep, dep);
    unsigned int ms = 0, son = 0;
    siz[x] = 1;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to != f)
        {
            const auto [sum, mx] = dfsChain<false>(to, x, dep + 1);
            siz[x] += siz[to];
            ret.first += sum;
            ret.second = max(ret.second, mx);
            if constexpr (isRoot)
            {
                if (siz[to] > ms)
                {
                    ms = siz[to];
                    son = to;
                }
            }
        }
    }
    if constexpr (isRoot)
    {
        if (ms * 2 == siz[x])
            ret.second = mdep[son];
    }
    else
        mdep[x] = ret.second;
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    dfsCentroid(1, 0, n);
    for (unsigned int i = 1; i <= n; ++i)
        if (i != centroid[0] && i != *(ccur - 1))
            cout << "-1\n";
        else
        {
            const auto [sum, mx] = dfsChain<true>(i, 0, 0);
            cout << sum * 2 - mx << "\n";
        }
    return 0;
}