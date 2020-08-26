#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <set>
using namespace std;
const unsigned int maxn = 100000;
using set_t = set<unsigned int>;

struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxn * 2 + 1];
const Edge* head[maxn + 1];
unsigned long long dist[maxn + 1], ans = 0;
unsigned int dfn[maxn + 1], rev[maxn + 1];
bool key[maxn + 1];
namespace lca
{
    unsigned int dep[maxn + 1], top[maxn + 1], son[maxn + 1], fa[maxn + 1];
    void dfsChain(const unsigned int x, const unsigned int tp);
    unsigned int lca(unsigned int a, unsigned int b);
    inline unsigned long long distance(unsigned int a, unsigned int b);
}
set_t treasure;

inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur++;
}
unsigned int dfsNo(const unsigned int x, const unsigned int f)
{
    using namespace lca;
    static unsigned int sta = 0;
    fa[x] = f;
    dep[x] = dep[f] + 1;
    dfn[x] = ++sta;
    rev[sta] = x;
    unsigned int ret = 1, ms = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dist[i->to] = dist[x] + i->val;
        const unsigned int sz = dfsNo(to, x);
        ret += sz;
        if (sz > ms)
        {
            ms = sz;
            son[x] = to;
        }
    }
    return ret;
}
namespace lca
{
    void dfsChain(const unsigned int x, unsigned int tp)
    {
        top[x] = tp;
        if (son[x])
            dfsChain(son[x], tp);
        for (const Edge* i = head[x]; i; i = i->pre)
            if (!top[i->to])
                dfsChain(i->to, i->to);
    }
    unsigned lca(unsigned int a, unsigned int b)
    {
        while (top[a] != top[b])
        {
            if (dep[top[a]] > dep[top[b]])
                a = fa[top[a]];
            else
                b = fa[top[b]];
        }
        return dep[a] < dep[b] ? a : b;
    }
    inline unsigned long long distance(unsigned int a, unsigned int b)
    {
        return dist[a] + dist[b] - 2 * dist[lca(a, b)];
    }
}
inline set_t::iterator next(set_t::iterator it)
{
    ++it;
    return it == treasure.end() ? treasure.begin() : it;
}
inline set_t::iterator pre(set_t::iterator it)
{
    return it == treasure.begin() ? --treasure.end() : --it;
}

void insert(const unsigned int x)
{
    const auto it = treasure.insert(dfn[x]).first;
    const unsigned int pv = rev[*pre(it)], nv = rev[*next(it)];
    ans = ans - lca::distance(pv, nv) + lca::distance(x, pv) + lca::distance(x, nv);
    key[x] = true;
}
void remove(const unsigned int x)
{
    const auto it = treasure.find(dfn[x]);
    const unsigned int pv = rev[*pre(it)], nv = rev[*(next(it))];
    if (treasure.size() > 2)
        ans = ans + lca::distance(pv, nv) - lca::distance(x, pv) - lca::distance(x, nv);
    else
        ans = 0;
    treasure.erase(it);
    key[x] = false;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y, z;
        cin >> x >> y >> z;
        addEdge(x, y, z);
        addEdge(y, x, z);
    }
    dfsNo(1, 0);
    lca::dfsChain(1, 1);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int p;
        cin >> p;
        if (key[p])
            remove(p);
        else
            insert(p);
        cout << ans << "\n";
    }
    return 0;
}
