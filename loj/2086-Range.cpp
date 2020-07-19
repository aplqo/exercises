#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5;

struct range
{
    unsigned int l, r;
    unsigned int len;
} rng[maxn + 1];

static void discratize(const unsigned int n)
{
    static unsigned int tmp[maxn * 2 + 1];
    unsigned int* p = tmp;
    for (range* i = rng; i < rng + n; ++i)
    {
        *(p++) = i->l;
        *(p++) = i->r;
    }
    sort(tmp, p);
    p = unique(tmp, p);
    for (range* i = rng; i < rng + n; ++i)
    {
        i->l = lower_bound(tmp, p, i->l) - tmp;
        i->r = lower_bound(tmp, p, i->r) - tmp;
    }
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
        rc = ++cur;
        lc->build(l, mv, cur);
        rc->build(mv, r, cur);
    }
    void modify(const unsigned int l, const unsigned int r, const int v)
    {
        if (l == lv && r == rv)
        {
            push(v);
            return;
        }
        pushdown();
        if (r <= mv)
            lc->modify(l, r, v);
        else if (l >= mv)
            rc->modify(l, r, v);
        else
        {
            lc->modify(l, mv, v);
            rc->modify(mv, r, v);
        }
        val = max(lc->val, rc->val);
    }

    int val;

private:
    void push(const unsigned int laz)
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

    STree *lc, *rc;
    unsigned int lv, mv, rv;
    int laz;
} SNodes[maxn * 2 * 4 + 1], *SRoot = SNodes;
static void init(const unsigned int n)
{
    discratize(n);
    sort(rng, rng + n, [](const range& a, const range& b) { return a.len < b.len; });
    STree* c = SNodes;
    SRoot->build(0, n * 2, c);
}
unsigned int solve(const unsigned int n, const unsigned int m)
{
    range* l = rng;
    unsigned int ret = UINT_MAX;
    for (range* r = rng; r < rng + n; ++r)
    {
        SRoot->modify(r->l, r->r + 1, 1);
        for (; l < rng + n && SRoot->val >= m; ++l)
        {
            SRoot->modify(l->l, l->r + 1, -1);
            ret = min(ret, r->len - l->len);
        }
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (range* i = rng; i < rng + n; ++i)
    {
        cin >> i->l >> i->r;
        i->len = i->r - i->l;
    }
    init(n);
    const unsigned int ans = solve(n, m);
    if (ans == UINT_MAX)
        cout << "-1" << endl;
    else
        cout << ans << endl;
    return 0;
}