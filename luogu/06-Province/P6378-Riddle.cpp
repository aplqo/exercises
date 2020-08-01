#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6, maxm = 1e6;
const unsigned int maxv = maxn * 2 * 2, maxe = maxm * 2 + maxn * 4 + maxn * 2;

struct edge
{
    struct vertex* to;
    const edge* pre;
} ed[maxe];
unsigned int sen;
struct vertex
{
    unsigned int sc = 0;
    unsigned int dfn = 0, low = 0;
    const edge* head = nullptr;

    void addEdge(vertex& to)
    {
        static edge* cur = ed;
        cur->to = &to;
        cur->pre = head;
        head = cur;
        ++cur;
    }
} ve[maxn][2], pre[maxn][2];

void tarjan(vertex* const x)
{
    static unsigned int sta, top;
    static vertex* stk[maxv + 1];
    stk[++top] = x;
    x->dfn = x->low = ++sta;
    for (const edge* i = x->head; i; i = i->pre)
    {
        vertex* const to = i->to;
        if (!to->dfn)
        {
            tarjan(to);
            x->low = min(x->low, to->low);
        }
        else if (!to->sc)
            x->low = min(x->low, to->dfn);
    }
    if (x->dfn == x->low)
    {
        ++sen;
        while (stk[top] != x)
            stk[top--]->sc = sen;
        stk[top--]->sc = sen;
    }
}
bool test(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        if (!ve[i][0].dfn)
            tarjan(&ve[i][0]);
        if (!ve[i][1].dfn)
            tarjan(&ve[i][1]);
        if (!pre[i][0].dfn)
            tarjan(&pre[i][0]);
        if (!pre[i][1].dfn)
            tarjan(&pre[i][1]);
    }
    for (unsigned int i = 0; i < n; ++i)
    {
        if (ve[i][0].sc == ve[i][1].sc)
            return false;
        if (pre[i][0].sc == pre[i][1].sc)
            return false;
    }
    return true;
}
void buildGraph(const unsigned int m, const unsigned int k)
{
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        --u;
        --v;
        ve[u][0].addEdge(ve[v][1]);
        ve[v][0].addEdge(ve[u][1]);
    }
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int w, lst;
        cin >> w >> lst;
        --lst;
        ve[lst][1].addEdge(pre[lst][1]);
        pre[lst][0].addEdge(ve[lst][0]);
        for (unsigned int j = 1; j < w; ++j)
        {
            unsigned int p;
            cin >> p;
            --p;
            pre[lst][1].addEdge(pre[p][1]);
            pre[p][0].addEdge(pre[lst][0]);
            ve[p][1].addEdge(pre[p][1]);
            pre[p][0].addEdge(ve[p][0]);
            pre[lst][1].addEdge(ve[p][0]);
            ve[p][1].addEdge(pre[lst][0]);
            lst = p;
        }
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, k;
    cin >> n >> m >> k;
    buildGraph(m, k);
    cout << (test(n) ? "TAK" : "NIE") << endl;
    return 0;
}