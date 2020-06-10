#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
using num = unsigned int;
const unsigned int maxn = 1e5;

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}

num mod;
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur), num dat[])
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        if (r - l == 1)
        {
            val = dat[l];
            return;
        }
        lc = ++cur;
        lc->build(l, mv, cur, dat);
        rc = ++cur;
        rc->build(mv, r, cur, dat);
        val = (lc->val + rc->val) % mod;
    }
    num query(const unsigned int l, const unsigned int r)
    {
        if (l == lv && r == rv)
            return val;
        if (laz)
        {
            lc->push(laz);
            rc->push(laz);
            laz = 0;
        }
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return (lc->query(l, mv) + rc->query(mv, r)) % mod;
    }
    void modify(const unsigned int l, const unsigned int r, const num val)
    {
        if (rv - lv == 1)
        {
            this->val = (this->val + val) % mod;
            return;
        }
        if (l == lv && r == rv)
        {
            push(val);
            return;
        }
        if (laz)
        {
            lc->push(laz);
            rc->push(laz);
            laz = 0;
        }
        if (r <= mv)
            lc->modify(l, r, val);
        else if (l >= mv)
            rc->modify(l, r, val);
        else
        {
            lc->modify(l, mv, val);
            rc->modify(mv, r, val);
        }
        this->val = (lc->val + rc->val) % mod;
    }

private:
    void push(const unsigned int l)
    {
        laz = (laz + l) % mod;
        val = (val + ((rv - lv) * l % mod)) % mod;
    }

    unsigned int lv, mv, rv;
    STree *lc, *rc;
    num val, laz = 0;
} SNode[maxn * 4 + 10], *SRoot = SNode;

unsigned int fa[maxn + 1], siz[maxn + 1], son[maxn + 1];
unsigned int dfn[maxn + 1], top[maxn + 1], dep[maxn + 1];
num val[maxn + 1], orig[maxn + 1];

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
    fa[x] = f;
    dep[x] = dep[f] + 1;
    unsigned int s = 0;
    for (edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        siz[x] += dfsSon(to, x);
        if (siz[to] > s)
        {
            s = siz[to];
            son[x] = to;
        }
    }
    return ++siz[x];
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

num queryChain(unsigned int x, unsigned int y)
{
    num ret = 0;
    while (top[x] != top[y])
    {
        if (dep[top[x]] > dep[top[y]])
        {
            ret = (ret + SRoot->query(dfn[top[x]], dfn[x] + 1)) % mod;
            x = fa[top[x]];
        }
        else
        {
            ret = (ret + SRoot->query(dfn[top[y]], dfn[y] + 1)) % mod;
            y = fa[top[y]];
        }
    }
    if (dep[x] > dep[y])
        swap(x, y);
    ret = (ret + SRoot->query(dfn[x], dfn[y] + 1)) % mod;
    return ret;
}
void modifyChain(unsigned int x, unsigned int y, const num val)
{
    while (top[x] != top[y])
    {
        if (dep[top[x]] > dep[top[y]])
        {
            SRoot->modify(dfn[top[x]], dfn[x] + 1, val);
            x = fa[top[x]];
        }
        else
        {
            SRoot->modify(dfn[top[y]], dfn[y] + 1, val);
            y = fa[top[y]];
        }
    }
    if (dep[x] > dep[y])
        swap(x, y);
    SRoot->modify(dfn[x], dfn[y] + 1, val);
}
inline num querySub(const unsigned int x)
{
    return SRoot->query(dfn[x], dfn[x] + siz[x]);
}
inline void modifySub(const unsigned int x, const num val)
{
    SRoot->modify(dfn[x], dfn[x] + siz[x], val);
}

void init(const unsigned int n, const unsigned int r)
{
    dfsSon(r, 0);
    dfsChain(r, r);
    for (unsigned int i = 1; i <= n; ++i)
        val[dfn[i]] = orig[i];
    STree* c = SNode;
    SRoot->build(1, n + 1, c, val);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, r;
    cin >> n >> m >> r >> mod;
    copy_n(istream_iterator<num>(cin), n, orig + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        addEdge(x, y);
        addEdge(y, x);
    }
    init(n, r);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int op, x, y;
        num z;
        cin >> op >> x;
        switch (op)
        {
        case 1:
            cin >> y >> z;
            modifyChain(x, y, z);
            break;
        case 2:
            cin >> y;
            cout << queryChain(x, y) << endl;
            break;
        case 3:
            cin >> z;
            modifySub(x, z);
            break;
        case 4:
            cout << querySub(x) << endl;
            break;
        }
    }
    return 0;
}