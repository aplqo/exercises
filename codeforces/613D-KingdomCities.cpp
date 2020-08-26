#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 100000;

struct Tree
{
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[maxn * 2 + 10], *cur = ed;
    const Edge* head[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to);
} orig;
class VirtTree : public Tree
{
    vector<unsigned int> undo;
    bool dirty[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to);

public:
    void build(const unsigned int n, unsigned int v[]);
    void clear();

    // test code
    void printTree(const unsigned int n)
    {
        for (unsigned int i = 1; i <= n; ++i)
            for (auto j = head[i]; j; j = j->pre)
                clog << i << " -> " << j->to << "\n";
        clog << flush;
    }

} virt;
bool hasKey[maxn + 1];
unsigned int dep[maxn + 1], dfn[maxn + 1], ans;
namespace lca
{
    unsigned int top[maxn + 1], son[maxn + 1], fa[maxn + 1];
    unsigned int lca(unsigned int a, unsigned int b);
}

inline void Tree::addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}
inline void VirtTree::addEdge(const unsigned int from, const unsigned int to)
{
    Tree::addEdge(from, to);
    if (!dirty[from])
    {
        dirty[from] = true;
        undo.push_back(from);
    }
    if (!dirty[to])
    {
        dirty[to] = true;
        undo.push_back(to);
    }
}
void VirtTree::build(const unsigned int n, unsigned int v[])
{
    static unsigned int stk[maxn + 1];
    unsigned int* top = stk;
    sort(v, v + n, [](unsigned int a, unsigned int b) { return dfn[a] < dfn[b]; });
    *(top++) = 1;
    for (const unsigned int* i = v + (v[0] == 1); i < v + n; ++i)
    {
        const unsigned int Lca = lca::lca(*(top - 1), *i);
        while (top - stk > 1 && dfn[*(top - 2)] > dfn[Lca])
        {
            addEdge(*(top - 2), *(top - 1));
            --top;
        }
        if (dfn[*(top - 1)] > dfn[Lca])
        {
            addEdge(Lca, *(top - 1));
            --top;
        }
        if (*(top - 1) != Lca)
            *(top++) = Lca;
        *(top++) = *i;
    }
    for (unsigned int* i = stk + 1; i < top; ++i)
        addEdge(*(i - 1), *i);
}
void VirtTree::clear()
{
    for (unsigned int i : undo)
    {
        hasKey[i] = false;
        dirty[i] = false;
        head[i] = nullptr;
    }
    cur = ed;
    undo.clear();
}
unsigned int dfsNo(const unsigned int x, const unsigned int f)
{
    static unsigned int sta = 0;
    dep[x] = dep[f] + 1;
    dfn[x] = ++sta;
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
bool dfsCheck(const unsigned int x)
{
    for (auto i = virt.head[x]; i; i = i->pre)
    {
        if (hasKey[x] && hasKey[i->to] && dep[i->to] - dep[x] < 2)
            return false;
        if (!dfsCheck(i->to))
            return false;
    }
    return true;
}
void dfsVirt(const unsigned int x)
{
    unsigned int cnt = 0;
    for (auto i = virt.head[x]; i; i = i->pre)
    {
        dfsVirt(i->to);
        cnt += hasKey[i->to];
    }
    if (hasKey[x])
        ans += cnt;
    else if (cnt > 1 && !hasKey[x])
        ++ans;
    hasKey[x] = hasKey[x] || (cnt == 1);
}
unsigned int query(const unsigned int n, unsigned int qbuf[])
{
    virt.clear();
    virt.build(n, qbuf);
    for_each(qbuf, qbuf + n, [](unsigned int i) { hasKey[i] = true; });
    if (!dfsCheck(1))
        return INT_MAX;
    ans = 0;
    dfsVirt(1);
    return ans;
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
        unsigned int u, v;
        cin >> u >> v;
        orig.addEdge(u, v);
        orig.addEdge(v, u);
    }
    dfsNo(1, 0);
    lca::dfsChain(1, 1);
    cin >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        static unsigned int qbuf[maxn + 1];
        unsigned int k;
        cin >> k;
        copy_n(istream_iterator<unsigned int>(cin), k, qbuf);
        const unsigned int v = query(k, qbuf);
        if (v == INT_MAX)
            cout << "-1"
                 << "\n";
        else
            cout << v << "\n";
    }
    return 0;
}