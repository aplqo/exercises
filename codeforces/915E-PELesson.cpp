#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
const unsigned int maxq = 3e5;

class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r, const unsigned int val[]);
    unsigned int assign(const unsigned int l, const unsigned int r, const bool val);

    static SegmentTree nodes[maxq * 4 * 2 + 10], *cur;

private:
    void pushdown();

    unsigned int lv, mv, rv;
    unsigned int sum, val;
    bool dirty, tag;
    SegmentTree *lc, *rc;
}* root = SegmentTree::nodes;
SegmentTree SegmentTree::nodes[maxq * 4 * 2 + 10], *SegmentTree::cur = SegmentTree::nodes + 1;
struct Query
{
    unsigned int l, r, k;
} qry[maxq + 10];
unsigned int val[maxq * 2 + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r, const unsigned int val[])
{
    lv = l;
    mv = (l + r) >> 1;
    rv = r;
    dirty = false;
    this->val = this->sum = val[r] - val[l];
    if (r - l == 1)
        return;
    lc = cur++;
    lc->build(l, mv, val);
    rc = cur++;
    rc->build(mv, r, val);
}
unsigned int SegmentTree::assign(const unsigned int l, const unsigned int r, const bool val)
{
    if (l == lv && r == rv)
    {
        dirty = true;
        tag = val;
        return this->val = val ? this->sum : 0;
    }
    pushdown();
    if (r <= mv)
        return this->val = lc->assign(l, r, val) + rc->val;
    else if (l >= mv)
        return this->val = lc->val + rc->assign(l, r, val);
    else
        return this->val = lc->assign(l, mv, val) + rc->assign(mv, r, val);
}
void SegmentTree::pushdown()
{
    if (!dirty)
        return;
    lc->dirty = rc->dirty = true;
    if (tag)
    {
        lc->val = lc->sum;
        rc->val = rc->sum;
    }
    else
        lc->val = rc->val = 0;
    lc->tag = rc->tag = this->tag;
    dirty = false;
}

static unsigned int discretize(const unsigned int q)
{
    unsigned int* cur = val;
    *(cur++) = 1;
    for (Query* i = qry; i < qry + q; ++i)
    {
        *(cur++) = i->l;
        *(cur++) = i->r;
    }
    std::sort(val, cur);
    cur = std::unique(val, cur);
    for (Query* i = qry; i < qry + q; ++i)
    {
        i->l = std::lower_bound(val, cur, i->l) - val;
        i->r = std::lower_bound(val, cur, i->r) - val;
    }
    return cur - val;
}
static void init(const unsigned int n, const unsigned int q)
{
    const unsigned int sz = discretize(q);
    val[sz] = n + 1;
    root->build(0, sz, val);
}

int main()
{
    std::ios::sync_with_stdio(false);
    unsigned int n, q;
    std::cin >> n >> q;
    for (Query* i = qry; i < qry + q; ++i)
    {
        std::cin >> i->l >> i->r >> i->k;
        ++(i->r);
    }
    init(n, q);
    for (const Query* i = qry; i < qry + q; ++i)
        std::cout << root->assign(i->l, i->r, i->k - 1) << "\n";
    return 0;
}