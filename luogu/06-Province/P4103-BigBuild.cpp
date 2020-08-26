#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1000000;
constexpr unsigned int inf = INT_MAX / 2;

namespace lca
{
    unsigned int top[maxn + 1], fa[maxn + 1], son[maxn + 1];
    void dfsChain(const unsigned int x, const unsigned int tp);
    unsigned int lca(unsigned int a, unsigned int b);
}
class Tree
{
public:
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[maxn * 2 + 1], *cur = ed;
    const Edge* head[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to);
} orig;
class VirtTree
{
public:
    struct Edge
    {
        unsigned int to, val;
        const Edge* pre;
    } ed[maxn * 2 + 1], *cur = ed;
    const Edge* head[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to);
    void build(const unsigned int n, unsigned int v[]);
} virt;
unsigned int dfn[maxn + 1], dep[maxn + 1], k;
unsigned int key[maxn + 1], ksta = 0;
unsigned int gf[maxn + 1], lf[maxn + 1], siz[maxn + 1];
unsigned int longest, shortest;
unsigned long long sum;

inline void Tree::addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}
inline void VirtTree::addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, dep[to] - dep[from], head[from] };
    head[from] = cur++;
}
void VirtTree::build(const unsigned int k, unsigned int v[])
{
    static unsigned int stk[maxn + 1];
    unsigned int* top = stk;
    sort(v, v + k, [](unsigned int a, unsigned int b) { return dfn[a] < dfn[b]; });
    *(top++) = 1;
    for (unsigned int* i = v + (v[0] == 1); i < v + k; ++i)
    {
        const unsigned int Lca = lca::lca(*i, *(top - 1));
        while (top - stk > 1 && dfn[*(top - 2)] > dfn[Lca])
        {
            addEdge(*(top - 2), *(top - 1));
            --top;
        }
        if (dfn[Lca] < dfn[*(top - 1)])
        {
            addEdge(Lca, *(top - 1));
            --top;
        }
        if (Lca != *(top - 1))
            *(top++) = Lca;
        *(top++) = *i;
    }
    for (unsigned int* i = stk + 1; i < top; ++i)
        addEdge(*(i - 1), *i);
}
unsigned int dfsNo(const unsigned int x, const unsigned int f)
{
    static unsigned int sta = 0;
    dfn[x] = ++sta;
    dep[x] = dep[f] + 1;
    lca::fa[x] = f;
    unsigned int ret = 1, ms = 0;
    for (auto i = orig.head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        const unsigned int sz = dfsNo(to, x);
        ret += sz;
        if (sz > ms)
        {
            ms = sz;
            lca::son[x] = to;
        }
    }
    return ret;
}
namespace lca
{
    void dfsChain(const unsigned int x, const unsigned int tp)
    {
        top[x] = tp;
        if (son[x])
            dfsChain(son[x], tp);
        for (auto i = orig.head[x]; i; i = i->pre)
            if (!top[i->to])
                dfsChain(i->to, i->to);
    }
    unsigned int lca(unsigned int a, unsigned int b)
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
}
void dfsVirt(const unsigned int x)
{
    if (key[x] == ksta)
    {
        siz[x] = 1;
        lf[x] = gf[x] = 0;
    }
    else
    {
        siz[x] = 0;
        lf[x] = inf;
        gf[x] = 0;
    }
    unsigned int cnt = 0;
    for (auto i = virt.head[x]; i; i = i->pre)
    {
        dfsVirt(i->to);
        siz[x] += siz[i->to];
        ++cnt;
    }
    if (key[x] == ksta || siz[x] < k || cnt > 1)
    {
        for (auto i = virt.head[x]; i; i = i->pre)
        {
            sum += 1ull * siz[i->to] * (k - siz[i->to]) * i->val;
            longest = max(longest, gf[x] + gf[i->to] + i->val);
            shortest = min(shortest, lf[x] + lf[i->to] + i->val);
            gf[x] = max(gf[x], gf[i->to] + i->val);
            lf[x] = min(lf[x], lf[i->to] + i->val);
        }
    }
    virt.head[x] = nullptr;
}
void query(const unsigned int k, unsigned int qbuf[])
{
    ++ksta;
    for_each(qbuf, qbuf + k, [](unsigned int i) { key[i] = ksta; });
    virt.build(k, qbuf);
    shortest = inf;
    longest = sum = 0;
    dfsVirt(1);
    virt.cur = virt.ed;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        orig.addEdge(a, b);
        orig.addEdge(b, a);
    }
    dfsNo(1, 0);
    lca::dfsChain(1, 1);
    cin >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        static unsigned int qbuf[maxn + 1];
        cin >> k;
        copy_n(istream_iterator<unsigned int>(cin), k, qbuf);
        query(k, qbuf);
        cout << sum << " " << shortest << " " << longest << "\n";
    }
    return 0;
}