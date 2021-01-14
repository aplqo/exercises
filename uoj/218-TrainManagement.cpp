#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5, maxm = 5e5, maxl = 19;
const unsigned int maxNode = maxn * maxl * 4;

struct Train
{
    unsigned int weight, time;
};
struct Range
{
    unsigned int l, r;
};
class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r);
    unsigned long long sum(const Range nodeRange, const Range queryRange) const;
    void assign(const SegmentTree* const old, const Range cur, const Range qry, const Train& val);
    Train get(const Range cur, const unsigned int pos) const;

    static SegmentTree mem[maxNode + 10], *cur;

private:
    inline void pushTag(const SegmentTree* old, const Range node, const Train& tag);
    inline unsigned int getSum(const Range rg) const
    {
        return val.time ? val.weight * (rg.r - rg.l) : val.weight;
    }

    const SegmentTree *lc, *rc;
    Train val;
};
SegmentTree SegmentTree::mem[maxNode + 10], *SegmentTree::cur = SegmentTree::mem;
const SegmentTree* root[maxm + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r)
{
    val = Train {};
    if (r - l == 1)
        return;
    const unsigned int mv = (l + r) >> 1;
    lc = cur;
    (cur++)->build(l, mv);
    rc = cur;
    (cur++)->build(mv, r);
}
unsigned long long SegmentTree::sum(const Range node, const Range query) const
{
    if (val.time)
        return (min(node.r, query.r) - max(node.l, query.l)) * val.weight;
    else if (query.l <= node.l && node.r <= query.r)
        return val.weight;
    const unsigned int mv = (node.l + node.r) >> 1;
    if (query.r <= mv)
        return lc->sum(Range { node.l, mv }, query);
    else if (query.l >= mv)
        return rc->sum(Range { mv, node.r }, query);
    else
        return lc->sum(Range { node.l, mv }, query)
            + rc->sum(Range { mv, node.r }, query);
}
void SegmentTree::assign(const SegmentTree* const old, const Range node, const Range query, const Train& v)
{
    if (query.l <= node.l && node.r <= query.r)
    {
        pushTag(old, node, v);
        return;
    }
    else
        val = Train {};
    const unsigned int mv = (node.l + node.r) >> 1;
    const Range lRange = Range { node.l, mv }, rRange { mv, node.r };
    const auto subtreeAssign = [this, query](const SegmentTree* old, const Range node, const Train& tag, const Train& v) {
        SegmentTree* tp = cur++;
        if (!tag.time)
            tp->assign(old, node, query, v);
        else
        {
            SegmentTree virt;
            virt.pushTag(old, node, tag);
            tp->assign(&virt, node, query, v);
        }
        return tp;
    };
    const auto fromOld = [this](const SegmentTree* old, const Range node, const Train& tag) -> const SegmentTree* {
        if (!tag.time)
            return old;
        SegmentTree* tp = cur++;
        tp->pushTag(old, node, tag);
        return tp;
    };
    if (query.l < mv)
        lc = subtreeAssign(old->lc, lRange, old->val, v);
    else
        lc = fromOld(old->lc, lRange, old->val);
    if (mv < query.r)
        rc = subtreeAssign(old->rc, rRange, old->val, v);
    else
        rc = fromOld(old->rc, rRange, old->val);
    val.weight = lc->getSum(lRange) + rc->getSum(rRange);
}
Train SegmentTree::get(const Range node, const unsigned int pos) const
{
    if (node.r - node.l == 1 || val.time)
        return val;
    const unsigned int mv = (node.l + node.r) >> 1;
    if (pos < mv)
        return lc->get(Range { node.l, mv }, pos);
    else
        return rc->get(Range { mv, node.r }, pos);
}
inline void SegmentTree::pushTag(const SegmentTree* old, const Range node, const Train& tag)
{
    lc = old->lc;
    rc = old->rc;
    val = tag;
}
inline void pop(const unsigned int time, const Range full, const unsigned int pos)
{
    const Train v = root[time - 1]->get(full, pos);
    if (!v.time)
        root[time] = root[time - 1];
    else
    {
        root[time] = SegmentTree::cur;
        (SegmentTree::cur++)->assign(root[time - 1], full, Range { pos, pos + 1 }, root[v.time - 1]->get(full, pos));
    }
}
inline void push(const unsigned int time, const Range full, const Range query, const unsigned int val)
{
    root[time] = SegmentTree::cur;
    (SegmentTree::cur++)->assign(root[time - 1], full, query, Train { val, time });
}

int main()
{
#ifndef APTEST
    ios::sync_with_stdio(false);
#endif
    unsigned int n, m, ty;
    cin >> n >> m >> ty;
    const Range full { 1, n + 1 };
    {
        root[0] = SegmentTree::cur;
        (SegmentTree::cur++)->build(1, n + 1);
    }
    unsigned long long ans = 0;
    const auto decode = [n, ty, &ans](unsigned int v) { return (v + ty * ans) % n + 1; };
    for (unsigned int i = 1; i <= m; ++i)
    {
        unsigned int op, l, r, x;
        cin >> op >> l;
        l = decode(l);
        switch (op)
        {
        case 1:
            root[i] = root[i - 1];
            cin >> r;
            r = decode(r);
            if (l > r)
                swap(l, r);
            cout << (ans = root[i]->sum(full, Range { l, r + 1 })) << "\n";
            break;
        case 2:
            pop(i, full, l);
            break;
        case 3:
            cin >> r >> x;
            r = decode(r);
            if (l > r)
                swap(l, r);
            push(i, full, Range { l, r + 1 }, x);
            break;
        }
    }
    return 0;
}