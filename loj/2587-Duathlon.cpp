#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000, maxm = 200000;
const unsigned int maxv = maxn * 2, maxe = maxm + maxn;

template <unsigned int V, unsigned int E>
class Graph
{
public:
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[E * 2 + 1], *cur = ed;
    const Edge* head[V + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to)
    {
        new (cur) Edge { to, head[from] };
        head[from] = cur++;
        new (cur) Edge { from, head[to] };
        head[to] = cur++;
    }
};
Graph<maxn, maxe> orig;
Graph<maxv, maxe> tree;
unsigned int vCnt, n;
unsigned long long ans;

namespace tarjan
{
    unsigned int dfn[maxn + 1], low[maxn + 1], sta = 0;
    void tarjan(const unsigned int x, const unsigned int f)
    {
        static unsigned int stk[maxv + 1], *top = stk;
        *(top++) = x;
        dfn[x] = low[x] = ++sta;
        for (auto i = orig.head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (to == f)
                continue;
            if (!dfn[to])
            {
                tarjan(to, x);
                low[x] = min(low[x], low[to]);
                if (dfn[x] <= low[to])
                {
                    const unsigned int sp = ++vCnt;
                    while (*(top - 1) != to)
                        tree.addEdge(sp, *(--top));
                    tree.addEdge(sp, *(--top));
                    tree.addEdge(sp, x);
                }
            }
            else
                low[x] = min(low[x], dfn[to]);
        }
    }
}
unsigned int siz[maxv + 1], vertexNum;
inline void countSquare(const unsigned int x, const unsigned int f, const unsigned int cnt)
{
    if (f)
        ans += static_cast<unsigned long long>(vertexNum - siz[x]) * siz[x] * (cnt - 2);
    for (auto j = tree.head[x]; j; j = j->pre)
        if (j->to != f)
            ans += static_cast<unsigned long long>(vertexNum - siz[j->to]) * siz[j->to] * (cnt - 2);
}
void countRound(const unsigned int x, const unsigned int f)
{
    unsigned long long t = 0;
    ans += 2 * static_cast<unsigned long long>(vertexNum - siz[x]) * (siz[x] - 1);
    for (auto j = tree.head[x]; j; j = j->pre)
        if (j->to != f)
        {
            ans += 2 * t * siz[j->to];
            t += siz[j->to];
        }
}
void dfs(const unsigned int x, const unsigned int f)
{
    siz[x] = x <= n;
    unsigned int cnt = 0;
    for (auto j = tree.head[x]; j; j = j->pre, ++cnt)
        if (j->to != f)
        {
            dfs(j->to, x);
            siz[x] += siz[j->to];
        }
    if (x > n)
        countSquare(x, f, cnt);
    else
        countRound(x, f);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        orig.addEdge(a, b);
    }
    vCnt = n;
    for (unsigned int i = 1; i <= n; ++i)
        if (!tarjan::dfn[i])
        {
            const unsigned int olds = tarjan::sta;
            tarjan::tarjan(i, 0);
            vertexNum = tarjan::sta - olds;
            dfs(i, 0);
        }
    cout << ans << "\n";
    return 0;
}