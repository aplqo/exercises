#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 3e4, maxm = 1e5, maxq = 4e4;

struct TreeEdge
{
    unsigned int to;
    const TreeEdge* pre;
} ed[maxn + 10];
const TreeEdge* head[maxn + 10];
list<unsigned int> graph[maxn + 10];
unordered_map<unsigned int, list<unsigned int>::iterator> edge[maxn + 10];
class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur));
    void clear(const unsigned int l, const unsigned int r);
    unsigned int count(const unsigned int l, const unsigned int r) const;

private:
    SegmentTree *lc, *rc;
    unsigned int lv, mv, rv;
    unsigned int val;
} nodes[maxn * 4 + 10], *root = nodes;
struct Query
{
    bool typ; // true: add, false: remove
    unsigned int u, v, ans;
} qry[maxq + 10];

inline void addTreeEdge(const unsigned int from, const unsigned int to)
{
    static TreeEdge* cur = ed;
    new (cur) TreeEdge { to, head[from] };
    head[from] = cur++;
}
inline void addGraphEdge(const unsigned int from, const unsigned int to)
{
    graph[from].push_back(to);
    edge[from][to] = prev(graph[from].end());
}
void dfs(const unsigned int x, const unsigned int f)
{
    static bool vis[maxn + 10];
    vis[x] = true;
    for (auto it = graph[x].begin(); it != graph[x].end();)
    {
        const unsigned int to = *it;
        if (to == f || !vis[to])
            it = graph[x].erase(it);
        else
            ++it;
        if (vis[to])
            continue;
        addTreeEdge(x, to);
        dfs(to, x);
    }
}
void SegmentTree::build(const unsigned int l, const unsigned int r, SegmentTree*(&cur))
{
    lv = l;
    mv = (l + r) >> 1;
    rv = r;
    val = r - l;
    if (r - l == 1)
        return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
}
void SegmentTree::clear(const unsigned int l, const unsigned int r)
{
    if ((l == lv && r == rv) || !val)
    {
        val = 0;
        return;
    }
    if (r <= mv)
        lc->clear(l, r);
    else if (l >= mv)
        rc->clear(l, r);
    else
    {
        lc->clear(l, mv);
        rc->clear(mv, r);
    }
    val = lc->val + rc->val;
}
unsigned int SegmentTree::count(const unsigned int l, const unsigned int r) const
{
    if ((l == lv && r == rv) || !val)
        return val;
    if (r <= mv)
        return lc->count(l, r);
    else if (l >= mv)
        return rc->count(l, r);
    else
        return lc->count(l, mv) + rc->count(mv, r);
}

namespace Chain
{
    unsigned int fa[maxn + 10], dep[maxn + 10], son[maxn + 10], top[maxn + 10], dfn[maxn + 10];

    unsigned int dfsSon(const unsigned int x)
    {
        unsigned int ret = 1, ms = 0;
        for (const TreeEdge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            dep[to] = dep[x] + 1;
            fa[to] = x;
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
        static unsigned int sta = 0;
        dfn[x] = ++sta;
        top[x] = tp;
        if (son[x])
            dfsChain(son[x], tp);
        for (const TreeEdge* i = head[x]; i; i = i->pre)
            if (i->to != son[x])
                dfsChain(i->to, i->to);
    }
    static void init(const unsigned int p)
    {
        dfsSon(p);
        dfsChain(p, p);
    }
    void clearPath(unsigned int u, unsigned int v)
    {
        while (top[u] != top[v])
        {
            if (dep[top[u]] > dep[top[v]])
                swap(u, v);
            root->clear(dfn[top[v]], dfn[v] + 1);
            v = fa[top[v]];
        }
        if (dep[u] > dep[v])
            swap(u, v);
        if (u != v)
            root->clear(dfn[u] + 1, dfn[v] + 1);
    }
    unsigned int countPath(unsigned int u, unsigned int v)
    {
        unsigned int ret = 0;
        while (top[u] != top[v])
        {
            if (dep[top[u]] > dep[top[v]])
                swap(u, v);
            ret += root->count(dfn[top[v]], dfn[v] + 1);
            v = fa[top[v]];
        }
        if (dep[u] > dep[v])
            swap(u, v);
        if (u != v)
            ret += root->count(dfn[u] + 1, dfn[v] + 1);
        return ret;
    }
}

void solveQuery(const unsigned int q)
{
    for (Query* i = qry + q - 1; i >= qry; --i)
    {
        if (i->typ)
            Chain::clearPath(i->u, i->v);
        else
            i->ans = Chain::countPath(i->u, i->v);
    }
}
static void init(const unsigned int n)
{
    dfs(1, 0);
    Chain::init(1);
    {
        SegmentTree* c = nodes;
        root->build(1, n + 1, c);
    }
    for (unsigned int i = 1; i < n; ++i)
        for (unsigned int j : graph[i])
            if (j > i)
                Chain::clearPath(i, j);
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
        addGraphEdge(u, v);
        addGraphEdge(v, u);
    }
    Query* cur = qry;
    while (true)
    {
        int op;
        cin >> op;
        if (op == -1)
            break;
        cin >> cur->u >> cur->v;
        if (!op)
        {
            cur->typ = true;
            graph[cur->u].erase(edge[cur->u][cur->v]);
            graph[cur->v].erase(edge[cur->v][cur->u]);
        }
        else
            cur->typ = false;
        ++cur;
    }
    init(n);
    solveQuery(cur - qry);
    for (const Query* i = qry; i < cur; ++i)
        if (!i->typ)
            cout << i->ans << "\n";
    return 0;
}