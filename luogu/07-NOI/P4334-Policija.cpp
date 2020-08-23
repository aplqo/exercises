#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 100000, maxe = 500000;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxe * 2 + 1];
const Edge* head[maxn + 1];
vector<unsigned int> chl[maxn + 1];
unsigned int dfn[maxn + 1], low[maxn + 1], finish[maxn + 1], dep[maxn + 1];

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
    dfn[x] = low[x] = ++sta;
    dep[x] = dep[f] + 1;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        if (!dfn[to])
        {
            dfs(to, x);
            low[x] = min(low[x], low[to]);
            chl[x].push_back(to);
        }
        else
            low[x] = min(low[x], dfn[to]);
    }
    finish[x] = ++sta;
}
bool isChild(const unsigned int x, const unsigned int f)
{
    if (x == f)
        return true;
    return dfn[f] < dfn[x] && finish[x] < finish[f];
}
unsigned int findSubtree(const unsigned int x, const unsigned int rt)
{
    unsigned int l = 0, r = chl[rt].size() - 1;
    while (l != r)
    {
        const unsigned int m = (l + r) >> 1, mid = chl[rt][m];
        if (dfn[x] > finish[mid])
            l = m + 1;
        if (finish[x] < dfn[mid])
            r = m - 1;
        if (x == mid || (dfn[mid] < dfn[x] && finish[x] < finish[mid]))
            return mid;
    }
    return chl[rt][l];
}
bool workAround(const unsigned int x, const unsigned int blk)
{
    return low[x] < dfn[blk];
}
bool testEdge(unsigned int a, unsigned int b, unsigned int ex, unsigned int ey)
{
    if (dep[ex] > dep[ey])
        swap(ex, ey);
    if (dep[ey] != dep[ex] + 1 || low[ey] < dfn[ey]) // not bridge
        return true;
    if (!(isChild(a, ey) ^ isChild(b, ey)))
        return true;
    else
        return false;
}
bool testVertex(unsigned int a, unsigned int b, unsigned int c)
{
    if (!isChild(a, c) && !isChild(b, c))
        return true;
    if (isChild(a, c) && isChild(b, c))
    {
        const unsigned int fa = findSubtree(a, c), fb = findSubtree(b, c);
        if (fa == fb)
            return true;
        else if (workAround(fa, c) && workAround(fb, c))
            return true;
        return false;
    }
    else
        return workAround(findSubtree(isChild(a, c) ? a : b, c), c);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, e, q;
    cin >> n >> e;
    for (unsigned int i = 0; i < e; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfs(1, 0);
    cin >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int typ, a, b;
        cin >> typ >> a >> b;
        if (typ == 1)
        {
            unsigned int g1, g2;
            cin >> g1 >> g2;
            cout << (testEdge(a, b, g1, g2) ? "yes" : "no") << endl;
        }
        else
        {
            unsigned int c;
            cin >> c;
            cout << (testVertex(a, b, c) ? "yes" : "no") << endl;
        }
    }
    return 0;
}