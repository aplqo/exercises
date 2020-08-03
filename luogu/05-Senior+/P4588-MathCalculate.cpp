#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxq = 100000;

unsigned long long mod;
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur));
    void dump(unsigned long long buf[]);
    void multply(const unsigned int l, const unsigned int r, const unsigned long long v);

private:
    unsigned int lv, rv, mv;
    STree *lc, *rc;
    unsigned long long val;
} Snodes[maxq * 4 + 1], *SRoot = Snodes;
void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    val = 1;
    if (r - l == 1)
        return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
}
void STree::dump(unsigned long long buf[])
{
    if (rv - lv == 1)
    {
        buf[lv] = val;
        return;
    }
    lc->val = (lc->val * val) % mod;
    rc->val = (rc->val * val) % mod;
    lc->dump(buf);
    rc->dump(buf);
}
void STree::multply(const unsigned int l, const unsigned int r, const unsigned long long v)
{
    if (l == lv && r == rv)
    {
        val = (val * v) % mod;
        return;
    }
    if (r <= mv)
        lc->multply(l, r, v);
    else if (l >= mv)
        rc->multply(l, r, v);
    else
    {
        lc->multply(l, mv, v);
        rc->multply(mv, r, v);
    }
}
unsigned long long buf[maxq + 1], rat[maxq + 1];

void solve()
{
    unsigned int q;
    cin >> q >> mod;
    {
        STree* c = Snodes;
        SRoot->build(0, q, c);
    }
    fill(rat, rat + q, 1);
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int op;
        cin >> op;
        if (op == 1)
        {
            unsigned int x;
            cin >> x;
            x %= mod;
            rat[i] = x;
        }
        else
        {
            unsigned int x;
            cin >> x;
            SRoot->multply(x - 1, i, rat[x - 1]);
            rat[x - 1] = 1;
        }
    }
    for (unsigned int i = 0; i < q; ++i)
        if (rat[i] != 1)
            SRoot->multply(i, q, rat[i]);
    SRoot->dump(buf);
    copy(buf, buf + q, ostream_iterator<unsigned long long>(cout, "\n"));
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