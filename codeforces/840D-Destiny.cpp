#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxn = 3e5, maxl = 19;
const unsigned int maxNode = maxn * maxl * 2 + maxn + maxn * 4;

class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r);
    void increase(const SegmentTree* old, const unsigned int pos);
    unsigned int find(const SegmentTree* old, const unsigned int times) const;

    static SegmentTree mem[maxNode + 10], *cur;

private:
    unsigned int lv, mv, rv;
    const SegmentTree *lc, *rc;
    unsigned int val;
};
SegmentTree SegmentTree::mem[maxNode + 10], *SegmentTree::cur = SegmentTree::mem;
const SegmentTree* root[maxn + 10];
unsigned int a[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r)
{
    lv = l;
    mv = (l + r) >> 1;
    rv = r;
    val = 0;
    if (r - l == 1)
        return;
    lc = cur;
    (cur++)->build(l, mv);
    rc = cur;
    (cur++)->build(mv, r);
}
void SegmentTree::increase(const SegmentTree* old, const unsigned int pos)
{
    lv = old->lv;
    mv = old->mv;
    rv = old->rv;
    val = old->val + 1;
    if (rv - lv == 1)
        return;
    if (pos < mv)
    {
        lc = cur;
        (cur++)->increase(old->lc, pos);
        rc = old->rc;
    }
    else
    {
        lc = old->lc;
        rc = cur;
        (cur++)->increase(old->rc, pos);
    }
}
unsigned int SegmentTree::find(const SegmentTree* old, const unsigned int k) const
{
    if (val - old->val < k)
        return 0;
    if (rv - lv == 1)
        return lv;
    const unsigned int pl = lc->find(old->lc, k);
    return pl ? pl : rc->find(old->rc, k);
}

static void init(const unsigned int n)
{
    root[0] = SegmentTree::cur;
    (SegmentTree::cur++)->build(1, n + 1);
    for (unsigned int i = 1; i <= n; ++i)
    {
        root[i] = SegmentTree::cur;
        (SegmentTree::cur++)->increase(root[i - 1], a[i]);
    }
}

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int n, q;
    std::cin >> n >> q;
    std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, a + 1);
    init(n);
    for (unsigned int i = q; i; --i)
    {
        unsigned int l, r, k;
        std::cin >> l >> r >> k;
        const unsigned int v = root[r]->find(root[l - 1], (r + 1 - l) / k + 1);
        if (v)
            std::cout << v << "\n";
        else
            std::cout << "-1\n";
    }
    return 0;
}