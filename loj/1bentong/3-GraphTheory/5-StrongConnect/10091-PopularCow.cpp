#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1e4, maxm = 5e4;

struct edge
{
    unsigned int from, to;
    edge* pre = nullptr;
} ed[maxm + 10];
edge* head[maxn + 10];
unsigned int degree[maxn + 10], siz[maxn + 10];
unsigned int scc[maxn + 10];
unsigned int dfn[maxn + 10], low[maxn + 10], stac[maxn + 10], *top = stac;

inline void AddEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->from = from;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void tarjan(const unsigned int x)
{
    static unsigned int sta = 0, cnt = 0;
    dfn[x] = low[x] = ++sta;
    *(++top) = x;
    for (edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (!dfn[to])
        {
            tarjan(to);
            low[x] = min(low[x], low[to]);
        }
        else if (!scc[to])
            low[x] = min(low[x], dfn[to]);
    }
    if (low[x] == dfn[x])
    {
        scc[x] = cnt;
        while ((*top) != x)
        {
            scc[*top] = cnt;
            ++siz[cnt];
            --top;
        }
        ++siz[cnt];
        --top;
        ++cnt;
    }
}
inline unsigned int solve(const unsigned int n, const unsigned int m)
{
    for (edge* i = ed; i < ed + m; ++i)
        if (scc[i->from] != scc[i->to])
            ++degree[scc[i->from]];
    unsigned int ret = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (!degree[i] && siz[i])
        {
            if (ret)
                return 0;
            ret = siz[i];
        }
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
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        AddEdge(u, v);
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (!dfn[i])
            tarjan(i);
    cout << solve(n, m) << endl;
    return 0;
}
