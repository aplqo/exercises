#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
const unsigned int maxn = 2e5, maxm = 2e5;

class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur), long long dat[])
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
        rc = ++cur;
        lc->build(l, mv, cur, dat);
        rc->build(mv, r, cur, dat);
        this->val = max(lc->val, rc->val);
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
            return max(lc->query(l, mv), rc->query(mv, r));
    }
    void modify(const unsigned int l, const unsigned int r, const long long val)
    {
        if (l == lv && r == rv)
        {
            push(val);
            return;
        }
        pushdown();
        if (r <= mv)
            lc->modify(l, r, val);
        else if (l >= mv)
            rc->modify(l, r, val);
        else
        {
            lc->modify(l, mv, val);
            rc->modify(mv, r, val);
        }
        this->val = max(lc->val, rc->val);
    }

private:
    void push(const long long laz)
    {
        this->val += laz;
        this->laz += laz;
    }
    void pushdown()
    {
        if (!laz)
            return;
        lc->push(laz);
        rc->push(laz);
        laz = 0;
    }

    long long val, laz = 0;
    STree *lc = nullptr, *rc = nullptr;
    unsigned int lv, mv, rv;
} SNodes[maxn * 4 + 1], *SRoot = SNodes;
struct Race
{
    unsigned int l, r;
    long long val;
} r[maxm + 1];
long long cost[maxn + 1], f[maxn + 1];

static void init(const unsigned int n, const unsigned int m)
{
    sort(r, r + m, [](const Race& l, const Race& r) { return l.r < r.r; });
    STree* c = SNodes;
    transform(cost + 1, cost + 1 + n, cost + 1, [](long long i) { return -i; });
    SRoot->build(0, n + 1, c, cost);
}
long long dp(const unsigned int n, const unsigned int m)
{
    const Race* p = r;
    for (unsigned int i = 1; i <= n; ++i)
    {
        while (p < r + m && p->r == i)
        {
            SRoot->modify(0, p->l, p->val);
            ++p;
        }
        SRoot->modify(0, i, cost[i]);
        f[i] = max({ f[i - 1], SRoot->query(0, i), 0ll });
        SRoot->modify(i, i + 1, f[i] - SRoot->query(i, i + 1));
    }
    return *max_element(f, f + 1 + n);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<long long>(cin), n, cost + 1);
    for (Race* i = r; i < r + m; ++i)
        cin >> i->l >> i->r >> i->val;
    init(n, m);
    cout << dp(n, m) << endl;
    return 0;
}
