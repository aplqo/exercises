#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>

#include <cstdio>

using namespace std;
const unsigned int maxn = 100000;

enum
{
    In = 0,
    Out = 1
};
struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxn * 2];
const edge* head[maxn + 1];
unsigned int dfn[2][maxn + 1], siz[maxn + 1];
long long val[maxn + 1], treeDat[maxn + 1];

class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur), long long dat[])
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        laz = 0;
        if (r - l == 1)
        {
            val = dat[l];
            return;
        }
        lc = ++cur;
        rc = ++cur;
        lc->build(l, mv, cur, dat);
        rc->build(mv, r, cur, dat);
        this->val = lc->val + rc->val;
    }
    long long query(const unsigned int l, const unsigned int r)
    {
        if (l == lv && r == rv)
            return val;
        pushdown();
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return lc->query(l, mv) + rc->query(mv, r);
    }
    void modify(const unsigned int l, const unsigned int r, const long long delt)
    {
        if (l == lv && r == rv)
        {
            push(delt);
            return;
        }
        pushdown();
        if (r <= mv)
            lc->modify(l, r, delt);
        else if (l >= mv)
            rc->modify(l, r, delt);
        else
        {
            lc->modify(l, mv, delt);
            rc->modify(mv, r, delt);
        }
        this->val = lc->val + rc->val;
    }

private:
    void push(const long long l)
    {
        val += (rv - lv) * l;
        laz += l;
    }
    void pushdown()
    {
        if (laz == 0)
            return;
        lc->push(laz);
        rc->push(laz);
        laz = 0;
    }

    unsigned int lv, mv, rv;
    STree *lc, *rc;
    long long val, laz;
} SNodes[maxn * 2 * 4 + 1], *SRoot[2];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void dfs(const unsigned int x, const unsigned int f)
{
    static unsigned int sta[2] = {};
    siz[x] = 1;
    dfn[In][x] = ++sta[In];
    for (const edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            dfs(i->to, x);
            siz[x] += siz[i->to];
        }
    dfn[Out][x] = ++sta[Out];
}
static void init(const unsigned int n)
{
    dfs(1, 0);
    STree* cur = SNodes;
    for (unsigned int i = 1; i <= n; ++i)
        treeDat[dfn[In][i]] = val[i];
    SRoot[In] = cur;
    SRoot[In]->build(0, n + 1, cur, treeDat);

    for (unsigned int i = 1; i <= n; ++i)
        treeDat[dfn[Out][i]] = val[i];
    SRoot[Out] = ++cur;
    SRoot[Out]->build(0, n + 1, cur, treeDat);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<long long>(cin), n, val + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int from, to;
        cin >> from >> to;
        addEdge(from, to);
        addEdge(to, from);
    }
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int op, x;
        long long a;
        cin >> op >> x;
        switch (op)
        {
        case 1:
            cin >> a;
            SRoot[In]->modify(dfn[In][x], dfn[In][x] + 1, a);
            SRoot[Out]->modify(dfn[Out][x], dfn[Out][x] + 1, a);
            break;
        case 2:
            cin >> a;
            SRoot[In]->modify(dfn[In][x], dfn[In][x] + siz[x], a);
            SRoot[Out]->modify(dfn[Out][x] + 1 - siz[x], dfn[Out][x] + 1, a);
            break;
        case 3:
            cout << (SRoot[In]->query(1, dfn[In][x] + siz[x]) - SRoot[Out]->query(0, dfn[Out][x])) << endl;
            break;
        }
    }
    return 0;
}