#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5;
const unsigned int maxv = maxn + maxm, maxe = maxv + 1;

template <unsigned int v, unsigned int e>
struct Graph
{
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[e + 10];
    const Edge* head[v + 10];

    void clear()
    {
        memset(head, 0, sizeof(head));
        cur = ed;
    }
    inline void addEdge(const unsigned int from, const unsigned int to)
    {
        new (cur) Edge { to, head[from] };
        head[from] = cur++;
    }

private:
    Edge* cur = ed;
};
Graph<maxn, maxm * 2> orig;
Graph<maxv, maxe> rsTree;
unsigned int n, vcnt;

namespace Tarjan
{
    unsigned int dfn[maxn + 10], low[maxn + 10], sta;
    unsigned int stk[maxn + 10], *top = stk;

    void tarjan(const unsigned int x)
    {
        dfn[x] = low[x] = ++sta;
        *(top++) = x;
        for (auto i = orig.head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (!dfn[to])
            {
                tarjan(to);
                low[x] = min(low[x], low[to]);
                if (dfn[x] <= low[to])
                {
                    const unsigned int vp = ++vcnt;
                    while (*(top - 1) != to)
                        rsTree.addEdge(vp, *(--top));
                    rsTree.addEdge(vp, *(--top));
                    rsTree.addEdge(x, vp);
                }
            }
            else
                low[x] = min(low[x], dfn[to]);
        }
    }
}
namespace Lca
{
    unsigned int fa[maxv + 10], son[maxv + 10], top[maxv + 10], dep[maxv + 10];

    unsigned int dfsSon(const unsigned int x)
    {
        unsigned int ret = 1, ms = 0;
        for (auto i = rsTree.head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            fa[to] = x;
            dep[to] = dep[x] + 1;
            const unsigned int sz = dfsSon(to);
            ret += sz;
            if (sz > ms)
            {
                ms = sz;
                son[x] = to;
            }
        }
        return ret;
    }
    void dfsChain(const unsigned int x, const unsigned int tp)
    {
        top[x] = tp;
        if (son[x])
            dfsChain(son[x], tp);
        for (auto i = rsTree.head[x]; i; i = i->pre)
            if (i->to != son[x])
                dfsChain(i->to, i->to);
    }
    static void init(const unsigned int p)
    {
        dfsSon(p);
        dfsChain(p, p);
    }
    unsigned int lca(unsigned int a, unsigned int b)
    {
        while (top[a] != top[b])
        {
            if (dep[top[a]] > dep[top[b]])
                swap(a, b);
            b = fa[top[b]];
        }
        return dep[a] < dep[b] ? a : b;
    }
}
namespace VirtTree
{
    unsigned int dfn[maxv + 10], cutCnt[maxv + 10], sta = 0;

    static void dfsNo(const unsigned int x)
    {
        dfn[x] = ++sta;
        for (auto i = rsTree.head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            cutCnt[to] = cutCnt[x] + (to <= n);
            dfsNo(to);
        }
    }
    unsigned int countCut(const unsigned int s, unsigned int vert[])
    {
        static unsigned int stk[maxv + 10];
        unsigned int *top = stk, cnt = 0;
        sort(vert, vert + s, [](unsigned int a, unsigned int b) { return dfn[a] < dfn[b]; });
        *(top++) = vert[0];
        for (const unsigned int* i = vert + 1; i < vert + s; ++i)
        {
            const unsigned int lca = Lca::lca(*i, *(top - 1));
            while (top - stk > 1 && dfn[*(top - 2)] > dfn[lca])
            {
                cnt += cutCnt[*(top - 1)] - cutCnt[*(top - 2)];
                --top;
            }
            if (dfn[*(top - 1)] > dfn[lca])
            {
                cnt += cutCnt[*(top - 1)] - cutCnt[lca];
                --top;
            }
            if (*(top - 1) != lca)
                *(top++) = lca;
            *(top++) = *i;
        }
        for (const unsigned int* i = stk + 1; i < top; ++i)
            cnt += cutCnt[*i] - cutCnt[*(i - 1)];
        return cnt + (stk[0] <= n);
    }
}
unsigned int buf[maxn + 10];

static void init(const unsigned int n)
{
    vcnt = n;
    Tarjan::tarjan(1);
    Lca::init(1);
    VirtTree::dfsNo(1);
}
static inline void clean()
{
    orig.clear();
    rsTree.clear();
    Tarjan::sta = 0;
    Tarjan::top = Tarjan::stk;
    memset(Tarjan::dfn, 0, sizeof(Tarjan::dfn));
    memset(Lca::son, 0, sizeof(Lca::son));
    VirtTree::sta = 0;
}

void solve()
{
    unsigned int m, q;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        orig.addEdge(u, v);
        orig.addEdge(v, u);
    }
    cin >> q;
    init(n);
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int s;
        cin >> s;
        copy_n(istream_iterator<unsigned int>(cin), s, buf);
        cout << VirtTree::countCut(s, buf) - s << "\n";
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        solve();
        clean();
    }
    return 0;
}