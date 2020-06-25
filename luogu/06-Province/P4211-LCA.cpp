#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 50000, maxq = 50000;
constexpr unsigned int mod = 201314;

struct edge
{
    unsigned int to;
    edge* pre;
} ed[maxn + 1];
edge* head[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}

class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur))
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        val = laz = 0;
        if (r - l == 1)
            return;
        lc = ++cur;
        lc->build(l, mv, cur);
        rc = ++cur;
        rc->build(mv, r, cur);
    }
    unsigned int query(const unsigned int l, const unsigned int r)
    {
        if (rv - lv == 1 || (l == lv && r == rv))
            return val;
        pushdown();
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return (lc->query(l, mv) + rc->query(mv, r)) % mod;
    }
    void increase(const unsigned int l, const unsigned int r)
    {
        if (rv - lv == 1 || (l == lv && r == rv))
        {
            push(1);
            return;
        }
        pushdown();
        if (r <= mv)
            lc->increase(l, r);
        else if (l >= mv)
            rc->increase(l, r);
        else
        {
            lc->increase(l, mv);
            rc->increase(mv, r);
        }
        this->val = (lc->val + rc->val) % mod;
    }

private:
    void push(const int laz)
    {
        this->val = (this->val + mod + (static_cast<long long>(rv - lv) * laz) % mod) % mod;
        this->laz = (this->laz + laz) % mod;
    }
    void pushdown()
    {
        if (!laz)
            return;
        lc->push(laz);
        rc->push(laz);
        laz = 0;
    }

    STree *lc, *rc;
    unsigned int lv, mv, rv;
    unsigned int val, laz;
} Snodes[maxn * 4], *SRoot = Snodes;

unsigned int top[maxn + 1], son[maxn + 1];
unsigned int fa[maxn + 1], dfn[maxn + 1];
unsigned int dfsSon(const unsigned int x)
{
    unsigned int ret = 1, sz = 0;
    for (edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int s = dfsSon(i->to);
        ret += s;
        if (sz < s)
        {
            sz = s;
            son[x] = i->to;
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
    dfsSon(1);
    dfsChain(1, 1);
    STree* c = Snodes;
    SRoot->build(1, n + 1, c);
}
unsigned int query(unsigned int x)
{
    unsigned int ret = 0;
    while (x)
    {
        ret = (ret + SRoot->query(dfn[top[x]], dfn[x] + 1)) % mod;
        x = fa[top[x]];
    }
    return ret;
}
void increase(unsigned int x)
{
    while (x)
    {
        SRoot->increase(dfn[top[x]], dfn[x] + 1);
        x = fa[top[x]];
    }
}

struct Query
{
    unsigned int l, r, z;
    unsigned int before, after;
} qry[maxq + 1];
Query *seql[maxq + 1], *seqr[maxq + 1];
struct mem
{
    unsigned int get(const unsigned int x)
    {
        if (x == pos)
            return val;
        pos = x;
        return val = query(x);
    }
    unsigned int pos = 0, val = 0;
};

void solve(const unsigned int n, const unsigned int q)
{
    transform(qry, qry + q, seql, [](Query& i) { return &i; });
    copy(seql, seql + q, seqr);
    sort(seql, seql + q, [](const Query* a, const Query* b) { return a->l != b->l ? a->l < b->l : a->z < b->z; });
    sort(seqr, seqr + q, [](const Query* a, const Query* b) { return a->r != b->r ? a->r < b->r : a->z < b->z; });
    unsigned int pl = 0, pr = 0;
    for (unsigned int i = seql[0]->l; i <= n; ++i)
    {
        {
            mem m;
            for (; pl < q && seql[pl]->l == i; ++pl)
                seql[pl]->before = m.get(seql[pl]->z);
        }
        increase(i);
        {
            mem m;
            for (; pr < q && seqr[pr]->r == i; ++pr)
                seqr[pr]->after = m.get(seqr[pr]->z);
        }
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    for (unsigned int i = 2; i <= n; ++i)
    {
        cin >> fa[i];
        addEdge(++fa[i], i);
    }
    for (Query* i = qry; i < qry + q; ++i)
    {
        cin >> i->l >> i->r >> i->z;
        ++(i->l);
        ++(i->r);
        ++(i->z);
    }
    init(n);
    solve(n, q);
    for (Query* i = qry; i < qry + q; ++i)
        cout << (i->after + mod - i->before) % mod << endl;
    return 0;
}
