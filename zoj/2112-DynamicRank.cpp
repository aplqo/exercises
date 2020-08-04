#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxn = 50000, maxm = 10000;
using Range = pair<unsigned int, unsigned int>;

struct Oper
{
    union {
        struct
        {
            unsigned int pos, val;
        } chg;
        struct
        {
            unsigned int l, r, k;
            unsigned int ans;
        } qry;
    } dat;
    enum
    {
        Insert,
        Remove,
        Query
    } typ;
} op[maxn + maxm * 2 + 1];
Oper *seq[maxn + maxm * 2 + 1], *ql[maxn + maxm * 2 + 1], *qr[maxn + maxm * 2 + 1];
unsigned int orig[maxn + maxm + 1], dcnt;
unsigned int a[maxn + 1], vis[maxn + 1];

namespace ftree
{
#define lowbit(x) ((x) & -(x))
    int val[maxn + maxm + 1];
    bool vis[maxn + maxm + 1];
    unsigned int stk[maxn + maxm + 1], *top = stk;

    void modify(unsigned int p, const int delt)
    {
        if (!vis[p])
        {
            vis[p] = true;
            *(top++) = p;
        }
        for (; p <= dcnt + 1; p += lowbit(p))
            val[p] += delt;
    }
    unsigned int query(unsigned int p)
    {
        unsigned int ret = 0;
        for (; p; p -= lowbit(p))
            ret += val[p];
        return ret;
    }
    inline unsigned int sum(const unsigned int l, const unsigned int r)
    {
        return query(r) - query(l - 1);
    }
    void clear()
    {
        for (unsigned int* i = stk; i < top; ++i)
        {
            for (unsigned int j = *i; j <= dcnt + 1; j += lowbit(j))
                val[j] = 0;
            vis[*i] = false;
        }
        top = stk;
    }
#undef lowbit
}
static void discretize(const unsigned int cnt)
{
    unsigned int* oe = orig + 1;
    for (Oper* i = op; i < op + cnt; ++i)
        if (i->typ == Oper::Insert)
            *(oe++) = i->dat.chg.val;
    sort(orig + 1, oe);
    oe = unique(orig + 1, oe);
    dcnt = oe - orig;
    for (Oper* i = op; i < op + cnt; ++i)
        if (i->typ == Oper::Insert)
            i->dat.chg.val = lower_bound(orig + 1, oe, i->dat.chg.val) - orig;
}
void binary(const Range v, const Range q)
{
    if (v.second - v.first == 1)
    {
        const unsigned int nv = orig[v.first];
        for (unsigned int i = q.first; i < q.second; ++i)
            if (seq[i]->typ == Oper::Query)
                seq[i]->dat.qry.ans = nv;
        return;
    }
    const unsigned int mid = (v.first + v.second) >> 1;
    unsigned int pl = 0, pr = 0;
    for (unsigned int i = q.first; i < q.second; ++i)
    {
        if (seq[i]->typ == Oper::Insert || seq[i]->typ == Oper::Remove)
        {
            const auto& va = seq[i]->dat.chg;
            if (va.val < mid)
            {
                ftree::modify(va.pos, seq[i]->typ == Oper::Insert ? 1 : -1);
                ql[pl++] = seq[i];
            }
            else
                qr[pr++] = seq[i];
        }
        else
        {
            auto& va = seq[i]->dat.qry;
            const unsigned int c = ftree::sum(va.l, va.r);
            if (c < va.k)
            {
                qr[pr++] = seq[i];
                va.k -= c;
            }
            else
                ql[pl++] = seq[i];
        }
    }
    copy(ql, ql + pl, seq + q.first);
    copy(qr, qr + pr, seq + q.first + pl);
    ftree::clear();
    binary(Range(v.first, mid), Range(q.first, q.first + pl));
    binary(Range(mid, v.second), Range(q.first + pl, q.second));
}
static void init(const unsigned int cnt)
{
    static unsigned int a[maxn + 1];
    for (Oper* i = op; i < op + cnt; ++i)
    {
        auto& p = i->dat.chg;
        if (i->typ == Oper::Insert)
            a[p.pos] = p.val;
        else if (i->typ == Oper::Remove)
            p.val = a[p.pos];
    }
}
void solve()
{
    unsigned int n, m;
    cin >> n >> m;
    unsigned int qcnt = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        op[qcnt].typ = Oper::Insert;
        auto& v = op[qcnt++].dat.chg;
        v.pos = i + 1;
        cin >> v.val;
    }
    for (unsigned int i = 0; i < m; ++i)
    {
        char o;
        cin >> o;
        if (o == 'C')
        {
            cin >> op[qcnt].dat.chg.pos;
            op[qcnt++].typ = Oper::Remove;
            op[qcnt].typ = Oper::Insert;
            auto& v = op[qcnt].dat.chg;
            cin >> v.val;
            op[qcnt].dat.chg.pos = op[qcnt - 1].dat.chg.pos;
            ++qcnt;
        }
        else
        {
            op[qcnt].typ = Oper::Query;
            auto& v = op[qcnt++].dat.qry;
            cin >> v.l >> v.r >> v.k;
        }
    }
    discretize(qcnt);
    init(qcnt);
    transform(op, op + qcnt, seq, [](Oper& i) { return &i; });
    binary(Range(1, dcnt + 1), Range(0, qcnt));
    for (Oper* i = op; i < op + qcnt; ++i)
        if (i->typ == Oper::Query)
            cout << i->dat.qry.ans << endl;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
        solve();
    return 0;
}