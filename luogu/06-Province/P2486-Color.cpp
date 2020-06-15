#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5;

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxn * 2];
edge* head[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}

inline unsigned int fold(const unsigned int lval, const unsigned int rval, const unsigned int left, const unsigned int right)
{
    return lval + rval - (left == right ? 1 : 0);
}
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur), unsigned int dat[])
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        if (rv - lv == 1)
        {
            left = right = dat[l];
            val = 1;
            return;
        }
        lc = ++cur;
        lc->build(l, mv, cur, dat);
        rc = ++cur;
        rc->build(mv, r, cur, dat);
        pushup();
    }
    unsigned int query(const unsigned int l, const unsigned int r)
    {
        if (l == lv && r == rv)
            return val;
        pushdown();
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return fold(lc->query(l, mv), rc->query(mv, r));
    }
    unsigned int find(const unsigned int pos) const
    {
        if (rv - lv == 1 || laz)
            return left;
        return pos < mv ? lc->find(pos) : rc->find(pos);
    }
    void replace(const unsigned int l, const unsigned int r, const unsigned int val)
    {
        if (l == lv && r == rv)
        {
            push(val);
            return;
        }
        pushdown();
        if (r <= mv)
            lc->replace(l, r, val);
        else if (l >= mv)
            rc->replace(l, r, val);
        else
        {
            lc->replace(l, mv, val);
            rc->replace(mv, r, val);
        }
        pushup();
    }

private:
    unsigned int fold(const unsigned int lval, const unsigned int rval) const
    {
        return ::fold(lval, rval, lc->right, rc->left);
    }
    void push(const unsigned int laz)
    {
        this->laz = laz;
        left = right = laz;
        val = 1;
    }
    void pushup()
    {
        this->val = fold(lc->val, rc->val);
        left = lc->left;
        right = rc->right;
    }
    void pushdown()
    {
        if (!laz)
            return;
        lc->push(laz);
        rc->push(laz);
        laz = 0;
    }

    unsigned int lv, mv, rv;
    unsigned int left, right;
    STree *lc = nullptr, *rc = nullptr;
    unsigned int val = 0, laz = 0;
} SNode[maxn * 4], *SRoot = SNode;
unsigned int color[maxn + 1], raw[maxn + 1];

unsigned int fa[maxn + 1], dep[maxn + 1], dfn[maxn + 1];
unsigned int son[maxn + 1], top[maxn + 1];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
    fa[x] = f;
    dep[x] = dep[f] + 1;
    unsigned int ret = 1, s = 0;
    for (edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        const unsigned int sz = dfsSon(to, x);
        ret += sz;
        if (sz > s)
        {
            s = sz;
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
    for (edge* i = head[x]; i; i = i->pre)
        if (!dfn[i->to])
            dfsChain(i->to, i->to);
}
void init(const unsigned int n)
{
    dfsSon(1, 0);
    dfsChain(1, 1);
    for (unsigned int i = 1; i <= n; ++i)
        color[dfn[i]] = raw[i];
    STree* c = SNode;
    SRoot->build(1, n + 1, c, color);
}

void replace(unsigned int x, unsigned int y, const unsigned int val)
{
    while (top[x] != top[y])
    {
        if (dep[top[x]] > dep[top[y]])
        {
            SRoot->replace(dfn[top[x]], dfn[x] + 1, val);
            x = fa[top[x]];
        }
        else
        {
            SRoot->replace(dfn[top[y]], dfn[y] + 1, val);
            y = fa[top[y]];
        }
    }
    if (dep[x] > dep[y])
        swap(x, y);
    SRoot->replace(dfn[x], dfn[y] + 1, val);
}
unsigned int query(unsigned int x, unsigned int y)
{
    unsigned int lval = 0, rval = 0, ret;
    unsigned int left = 0, right = 0;
    while (top[x] != top[y])
    {
        if (dep[top[x]] > dep[top[y]])
        {
            lval = fold(lval, SRoot->query(dfn[top[x]], dfn[x] + 1), left, SRoot->find(dfn[x]));
            left = SRoot->find(dfn[top[x]]);
            x = fa[top[x]];
        }
        else
        {
            rval = fold(rval, SRoot->query(dfn[top[y]], dfn[y] + 1), right, SRoot->find(dfn[y]));
            right = SRoot->find(dfn[top[y]]);
            y = fa[top[y]];
        }
    }
    const unsigned int v = dep[x] > dep[y] ? SRoot->query(dfn[y], dfn[x] + 1) : SRoot->query(dfn[x], dfn[y] + 1);
    ret = fold(lval, v, left, SRoot->find(dfn[x]));
    ret = fold(ret, rval, SRoot->find(dfn[y]), right);
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<unsigned int>(cin), n, raw + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        char op;
        unsigned int a, b, c;
        cin >> op >> a >> b;
        if (op == 'C')
        {
            cin >> c;
            replace(a, b, c);
        }
        else
            cout << query(a, b) << endl;
    }
    return 0;
}