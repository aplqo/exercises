#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
using num = long long;
const unsigned int maxn = 500000;

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
        val = gcd(lc->val, rc->val);
    }
    void modify(const unsigned int pos, const num val)
    {
        if (rv - lv == 1)
        {
            this->val += val;
            return;
        }
        if (pos < mv)
            lc->modify(pos, val);
        else
            rc->modify(pos, val);
        this->val = gcd(lc->val, rc->val);
    }
    num query(const unsigned int l, const unsigned int r) const
    {
        if (l == lv && r == rv)
            return val;
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return gcd(lc->query(l, mv), rc->query(mv, r));
    }

private:
    unsigned int lv, mv, rv;
    STree *lc = nullptr, *rc = nullptr;
    num val;
} SNode[maxn * 2 + 1], *SRoot = SNode;
num dat[maxn + 2], dif[maxn + 2];

namespace FTree
{
#define lowbit(x) ((x) & -(x))
    num tree[maxn + 1];
    void modify(unsigned int p, const unsigned int n, const num val)
    {
        for (; p <= n; p += lowbit(p))
            tree[p] += val;
    }
    num query(unsigned int p)
    {
        num ret = 0;
        for (; p; p -= lowbit(p))
            ret += tree[p];
        return ret;
    }
#undef lowbit
}

void init(const unsigned int n)
{
    adjacent_difference(dat + 1, dat + 1 + n, dif + 1);
    STree* c = SNode;
    SRoot->build(1, n + 1, c, dif);
    for (unsigned int i = 1; i <= n; ++i)
        FTree::modify(i, n, dif[i]);
}
num query(const unsigned int l, const unsigned int r)
{
    return gcd(FTree::query(l), SRoot->query(l + 1, r + 1));
}
void inc(const unsigned int l, const unsigned int r, const unsigned int n, const num val)
{
    FTree::modify(l, n, val);
    SRoot->modify(l, val);
    if (r != n)
    {
        SRoot->modify(r + 1, -val);
        FTree::modify(r + 1, n, -val);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<num>(cin), n, dat + 1);
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        char op;
        unsigned int l, r;
        num d;
        cin >> op >> l >> r;
        if (op == 'C')
        {
            cin >> d;
            inc(l, r, n, d);
        }
        else
            cout << query(l, r) << endl;
    }
    return 0;
}
