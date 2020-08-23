#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e5, maxm = 2e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm * 2 + 1];
const Edge* head[maxn + 1];
unsigned int dfn[maxn + 1], low[maxn + 1], finish[maxn + 1], fa[maxn + 1], dep[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur++;
}
void dfs(const unsigned int x, const unsigned int f)
{
    static unsigned int sta = 0;
    fa[x] = f;
    dep[x] = dep[f] + 1;
    dfn[x] = low[x] = ++sta;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        if (!dfn[to])
        {
            dfs(to, x);
            low[x] = min(low[x], low[to]);
        }
        else
            low[x] = min(low[x], dfn[to]);
    }
    finish[x] = ++sta;
}
bool isChild(const unsigned int x, const unsigned int rt)
{
    return dfn[rt] <= dfn[x] && finish[x] <= finish[rt];
}
bool test(unsigned int a, unsigned int b, const unsigned int c)
{
    if (isChild(a, c) && isChild(b, c))
        return low[a] < dfn[c] && low[b] < dfn[c];
    else
        return isChild(a, c) ? low[a] < dfn[c] : low[b] < dfn[c];
}
unsigned int findVert(unsigned int a, unsigned int b)
{
    unsigned int ret = maxn + 1;
    while (a != b)
    {
        if (dep[a] < dep[b])
            swap(a, b);
        if (b != fa[a] && !test(a, b, fa[a]))
            ret = min(ret, fa[a]);
        a = fa[a];
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, a, b;
    cin >> n;
    while (true)
    {
        unsigned int u, v;
        cin >> u >> v;
        if (!u && !v)
            break;
        addEdge(u, v);
        addEdge(v, u);
    }
    cin >> a >> b;
    dfs(1, 0);
    const unsigned int t = findVert(a, b);
    if (t == maxn + 1)
        cout << "No solution" << endl;
    else
        cout << t << endl;
    return 0;
}