#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5;

class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur), const unsigned long long val[]);
    unsigned long long sum(const unsigned int l, const unsigned int r) const;
    void add(const unsigned int l, const unsigned int r, const unsigned long long delt);

private:
    unsigned int lv, mv, rv;
    SegmentTree *lc, *rc;
    unsigned long long val, laz;
} nodes[maxn * 4 + 10], *sroot = nodes;
struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn + 10];
const Edge* head[maxn + 10];
unsigned long long a[maxn + 10];
unsigned int n;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}
void SegmentTree::build(const unsigned int l, const unsigned int r, SegmentTree*(&cur), const unsigned long long val[])
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    laz = 0;
    if (r - l == 1)
    {
        this->val = val[l];
        return;
    }
    lc = ++cur;
    lc->build(l, mv, cur, val);
    rc = ++cur;
    rc->build(mv, r, cur, val);
    this->val = lc->val + rc->val;
}
unsigned long long SegmentTree::sum(const unsigned int l, const unsigned int r) const
{
    if (l == lv && r == rv)
        return val;
    if (r <= mv)
        return lc->sum(l, r) + (r - l) * laz;
    else if (l >= mv)
        return rc->sum(l, r) + (r - l) * laz;
    else
        return lc->sum(l, mv) + rc->sum(mv, r) + (r - l) * laz;
}
void SegmentTree::add(const unsigned int l, const unsigned int r, const unsigned long long delt)
{
    if (l == lv && r == rv)
    {
        laz += delt;
        val += (rv - lv) * delt;
        return;
    }
    if (r <= mv)
        lc->add(l, r, delt);
    else if (l >= mv)
        rc->add(l, r, delt);
    else
    {
        lc->add(l, mv, delt);
        rc->add(mv, r, delt);
    }
    this->val = lc->val + rc->val + (rv - lv) * laz;
}

namespace Subs
{
    unsigned int discover[maxn + 10], finish[maxn + 10];
    vector<unsigned int> chl[maxn + 10];

    inline bool isSubtree(const unsigned int x, const unsigned int rt)
    {
        return discover[rt] < discover[x] && finish[x] < finish[rt];
    }
    unsigned int findSubtree(const unsigned int x, const unsigned int rt)
    {
        unsigned int l = 0, r = chl[rt].size() - 1;
        while (l != r)
        {
            const unsigned int mid = (l + r) >> 1, mp = chl[rt][mid];
            if (finish[mp] < discover[x])
                l = mid + 1;
            else if (discover[mp] > finish[x])
                r = mid - 1;
            if (x == mp || (discover[mp] < discover[x] && finish[x] < finish[mp]))
                return mp;
        }
        return chl[rt][l];
    }
}
namespace Chain
{
    unsigned int dfn[maxn + 10], siz[maxn + 10], dep[maxn + 10];
    unsigned int top[maxn + 10], son[maxn + 10], fa[maxn + 10];

    void dfsSon(const unsigned int x)
    {
        siz[x] = 1;
        unsigned int ms = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            dep[to] = dep[x] + 1;
            fa[to] = x;
            dfsSon(to);
            siz[x] += siz[to];
            if (siz[to] > ms)
            {
                ms = siz[to];
                son[x] = to;
            }
        }
    }
    void dfsChain(const unsigned int x, const unsigned int tp)
    {
        using namespace Subs;
        static unsigned int sta = 0, sfsta = 0;
        dfn[x] = ++sta;
        top[x] = tp;
        discover[x] = ++sfsta;
        if (son[x])
        {
            dfsChain(son[x], tp);
            chl[x].emplace_back(son[x]);
        }
        for (const Edge* i = head[x]; i; i = i->pre)
            if (!dfn[i->to])
            {
                chl[x].emplace_back(i->to);
                dfsChain(i->to, i->to);
            }
        finish[x] = ++sfsta;
    }
    static void init(const unsigned int n, const unsigned int tp)
    {
        static unsigned long long da[maxn + 10];
        dfsSon(tp);
        dfsChain(tp, tp);
        for (unsigned int i = 1; i <= n; ++i)
            da[dfn[i]] = a[i];
        {
            SegmentTree* c = nodes;
            sroot->build(1, n + 1, c, da);
        }
    }
    template <class T>
    void processPath(unsigned int a, unsigned int b, T fun)
    {
        while (top[a] != top[b])
        {
            if (dep[top[a]] > dep[top[b]])
                swap(a, b);
            fun(dfn[top[b]], dfn[b] + 1);
            b = fa[top[b]];
        }
        if (dep[a] > dep[b])
            swap(a, b);
        fun(dfn[a], dfn[b] + 1);
    }
    template <class T>
    inline void processExclude(const unsigned int x, T fun)
    {
        if (dfn[x] != 1)
            fun(1, dfn[x]);
        if (dfn[x] + siz[x] < n + 1)
            fun(dfn[x] + siz[x], n + 1);
    }
    template <class T>
    void processSubtree(const unsigned int x, const unsigned int root, T fun)
    {
        if (x == root)
            fun(1, n + 1);
        else if (!Subs::isSubtree(root, x))
            fun(dfn[x], dfn[x] + siz[x]);
        else
            processExclude(Subs::findSubtree(root, x), fun);
    }
}

inline void addPath(unsigned int a, unsigned int b, const unsigned long long delt)
{
    Chain::processPath(a, b, [delt](unsigned int a, unsigned int b) { sroot->add(a, b, delt); });
}
inline unsigned long long sumPath(unsigned int a, unsigned int b)
{
    unsigned long long ret = 0;
    Chain::processPath(a, b, [&ret](unsigned int a, unsigned int b) { ret += sroot->sum(a, b); });
    return ret;
}
inline void addSubTree(const unsigned int x, const unsigned int rt, const unsigned long long delt)
{
    Chain::processSubtree(x, rt, [delt](unsigned int a, unsigned int b) { sroot->add(a, b, delt); });
}
inline unsigned long long sumSubTree(const unsigned int x, const unsigned int rt)
{
    unsigned long long ret = 0;
    Chain::processSubtree(x, rt, [&ret](unsigned int a, unsigned int b) { ret += sroot->sum(a, b); });
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int m, root = 1;
    cin >> n;
    copy_n(istream_iterator<unsigned long long>(cin), n, a + 1);
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int f;
        cin >> f;
        addEdge(f, i);
    }
    Chain::init(n, root);
    cin >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int op, ar1, ar2;
        unsigned long long delt;
        cin >> op;
        switch (op)
        {
        case 1:
            cin >> root;
            break;
        case 2:
            cin >> ar1 >> ar2 >> delt;
            addPath(ar1, ar2, delt);
            break;
        case 3:
            cin >> ar1 >> delt;
            addSubTree(ar1, root, delt);
            break;
        case 4:
            cin >> ar1 >> ar2;
            cout << sumPath(ar1, ar2) << "\n";
            break;
        case 5:
            cin >> ar1;
            cout << sumSubTree(ar1, root) << "\n";
            break;
        }
    }
    return 0;
}