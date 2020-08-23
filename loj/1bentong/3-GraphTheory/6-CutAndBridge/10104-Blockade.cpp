#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxm = 5e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 1];
unsigned int dfn[maxn + 1], low[maxn + 1], siz[maxn + 1];
unsigned long long ans[maxn + 1];
unsigned long long n;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur++;
}
void tarjan(const unsigned int x, const unsigned int root)
{
    static unsigned int sta = 0;
    dfn[x] = low[x] = ++sta;
    siz[x] = 1;
    unsigned int cnt = 0, sum = 0;
    bool cut = false;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (!dfn[to])
        {
            tarjan(to, root);
            ++cnt;
            low[x] = min(low[x], low[to]);
            siz[x] += siz[to];
            cut = cut || (x == root && cnt > 1) || (x != root && dfn[x] <= low[to]);
            if (dfn[x] <= low[to])
            {
                ans[x] += siz[to] * (n - siz[to]);
                sum += siz[to];
            }
        }
        else
            low[x] = min(low[x], dfn[to]);
    }
    if (cut)
        ans[x] += (n - 1 - sum) * (sum + 1) + n - 1;
    else
        ans[x] = 2 * (n - 1);
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
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    tarjan(1, 1);
    copy(ans + 1, ans + 1 + n, ostream_iterator<unsigned long long>(cout, "\n"));
    return 0;
}