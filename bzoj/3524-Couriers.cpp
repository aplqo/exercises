#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5, maxl = 19;
constexpr unsigned int npos = 0;

class SegmentTree
{
public:
    void build(const unsigned int l, const unsigned int r);
    void increase(const unsigned int l, const unsigned int r, const SegmentTree* old, const unsigned int pos);
    unsigned int find(const unsigned int l, const unsigned int r, const SegmentTree* old, const unsigned int len) const;

    static SegmentTree node[maxn * maxl * 4 + 100], *cur;

private:
    const SegmentTree *lc, *rc;
    unsigned int val;
};
SegmentTree SegmentTree::node[maxn * maxl * 4 + 100], *SegmentTree::cur = SegmentTree::node;
const SegmentTree* root[maxn + 10];

void SegmentTree::build(const unsigned int l, const unsigned int r)
{
    val = 0;
    lc = rc = this;
}
void SegmentTree::increase(const unsigned int l, const unsigned int r, const SegmentTree* old, const unsigned int pos)
{
    const unsigned int mv = (l + r) >> 1;
    val = old->val + 1;
    if (r - l == 1)
        return;
    if (pos < mv)
    {
        lc = ++cur;
        cur->increase(l, mv, old->lc, pos);
        rc = old->rc;
    }
    else
    {
        lc = old->lc;
        rc = ++cur;
        cur->increase(mv, r, old->rc, pos);
    }
}
unsigned int SegmentTree::find(const unsigned int l, const unsigned int r, const SegmentTree* old, const unsigned int len) const
{
    if (r - l == 1)
        return l;
    const unsigned int mv = (l + r) >> 1;
    if ((lc->val - old->lc->val) * 2 > len)
        return lc->find(l, mv, old->lc, len);
    else if ((rc->val - old->rc->val) * 2 > len)
        return rc->find(mv, r, old->rc, len);
    return npos;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    {
        root[0] = ++SegmentTree::cur;
        SegmentTree::cur->build(1, n + 1);
    }
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int v;
        cin >> v;
        root[i] = ++SegmentTree::cur;
        SegmentTree::cur->increase(1, n + 1, root[i - 1], v);
    }
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        cout << root[b]->find(1, n + 1, root[a - 1], b + 1 - a) << "\n";
    }
    return 0;
}