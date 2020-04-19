/* Luogu team T129726: Tree out of school*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num = unsigned int;
const int maxn = 500, maxk = 500;
const int maxl = maxn * maxk;

class tree
{
public:
    void build(const unsigned int l, const unsigned int r, num dat[], tree*(&cur))
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
        lc->build(lv, mv, dat, cur);
        rc = ++cur;
        rc->build(mv, rv, dat, cur);
        val = min(lc->val, rc->val);
    }
    num query(const unsigned int l, const unsigned int r)
    {
        if (l == lv && r == rv)
            return val;
        push();
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return min(lc->query(l, mv), rc->query(mv, r));
    }
    void replace(const unsigned int l, const unsigned int r, const num val)
    {
        if (rv - lv == 1)
        {
            this->val = val;
            return;
        }
        if (l == rv && r == rv)
        {
            flag = true;
            this->val = val;
            laz = val;
        }
        push();
        if (r <= mv)
            lc->replace(l, r, val);
        else if (l >= mv)
            rc->replace(l, r, val);
        else
        {
            lc->replace(l, mv, val);
            rc->replace(mv, r, val);
        }
        this->val = min(lc->val, rc->val);
    }

private:
    void pushr(num l)
    {
        val = l;
        laz = l;
        flag = true;
    }
    void push()
    {
        if (flag)
        {
            lc->pushr(laz);
            rc->pushr(laz);
            flag = false;
        }
    }

    unsigned int lv, rv, mv;
    num val = 0, laz = 0;
    bool flag = false;
    tree *lc = nullptr, *rc = nullptr;
} node[maxl * 4 + 10], *tr = node;
num a[maxl + 10];

void build(const unsigned int n, const unsigned int k)
{
    for (unsigned int i = 1; i < k; ++i)
        copy(a, a + n, a + n * i);
    tree* c = node;
    tr->build(0, n * k, a, c);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k, q;
    cin >> n >> k;
    for (num* i = a; i < a + n; ++i)
        cin >> *i;
    build(n, k);
    cin >> q;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int op, l, r, x;
        cin >> op >> l >> r;
        if (op == 1)
        {
            cin >> x;
            tr->replace(l - 1, r, x);
        }
        else
            cout << tr->query(l - 1, r) << endl;
    }
    return 0;
}