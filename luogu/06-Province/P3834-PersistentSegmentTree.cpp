#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2e5;

class STree
{
public:
    STree* build(const unsigned int l, const unsigned int r)
    {
        if (r - l == 1)
            return this;
        lc = create(l, mv)->build(l, mv);
        rc = create(mv, rv)->build(mv, r);
        return this;
    }
    STree* increase(const STree* old, const unsigned x)
    {
        if (rv - lv == 1)
        {
            this->val = old->val + 1;
            return this;
        }
        if (x < mv)
        {
            this->rc = old->rc;
            lc = create(lv, mv)->increase(old->lc, x);
        }
        else
        {
            this->lc = old->lc;
            rc = create(mv, rv)->increase(old->rc, x);
        }
        this->val = lc->val + rc->val;
        return this;
    }
    unsigned int find(const STree* old, const unsigned int k) const
    {
        if (rv - lv == 1)
            return lv;
        const unsigned int v = this->lc->val - old->lc->val;
        return k <= v ? lc->find(old->lc, k) : rc->find(old->rc, k - v);
    }

    static STree* cur;
    static inline STree* create(const unsigned int l, const unsigned int r)
    {
        (++cur)->init(l, r);
        return cur;
    }

private:
    void init(const unsigned int l, const unsigned int r)
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        val = 0;
        lc = rc = nullptr;
    }

    unsigned int lv, mv, rv;
    const STree *lc, *rc;
    unsigned int val;
} SNodes[maxn * 4 + maxn * 20 * 4];
STree* STree::cur = SNodes - 1;
const STree* root[maxn + 1];
int dat[maxn + 1], orig[maxn + 1];

static void discretize(const unsigned int n)
{
    copy(dat + 1, dat + 1 + n, orig);
    sort(orig, orig + n);
    int* const en = unique(orig, orig + n);
    for (int* i = dat + 1; i < dat + n + 1; ++i)
        *i = lower_bound(orig, orig + n, *i) - orig;
}
static void init(const unsigned int n)
{
    discretize(n);
    root[0] = STree::create(0, n)->build(0, n);
    for (unsigned int i = 1; i <= n; ++i)
        root[i] = STree::create(0, n)->increase(root[i - 1], dat[i]);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<int>(cin), n, dat + 1);
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r, k;
        cin >> l >> r >> k;
        cout << orig[root[r]->find(root[l - 1], k)] << endl;
    }
    return 0;
}