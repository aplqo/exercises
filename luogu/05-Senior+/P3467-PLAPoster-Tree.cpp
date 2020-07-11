#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 250000;

class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur), unsigned int dat[])
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        laz = 0;
        if (r - l == 1)
        {
            lVal = gVal = dat[l];
            return;
        }
        lc = ++cur;
        lc->build(l, mv, cur, dat);
        rc = ++cur;
        rc->build(mv, r, cur, dat);
        this->lVal = min(lc->lVal, rc->lVal);
        this->gVal = max(lc->gVal, rc->gVal);
    }
    unsigned int query(const unsigned int l, const unsigned int r)
    {
        if (l == lv && r == rv)
            return lVal;
        pushdown();
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return min(lc->query(l, mv), rc->query(mv, r));
    }
    void minus(const unsigned int l, const unsigned int r, const unsigned int val)
    {
        if (l == lv && r == rv)
        {
            push(val);
            return;
        }
        pushdown();
        if (r <= mv)
            lc->minus(l, r, val);
        else if (l >= mv)
            rc->minus(l, r, val);
        else
        {
            lc->minus(l, mv, val);
            rc->minus(mv, r, val);
        }
        this->lVal = min(lc->lVal, rc->lVal);
        this->gVal = max(lc->gVal, rc->gVal);
    }
    pair<unsigned int, unsigned int> find()
    {
        static const auto empty = [](pair<unsigned int, unsigned int> v) {
            return v.first == v.second;
        };
        if (!gVal)
            return make_pair(rv, rv);
        if (rv - lv == 1)
            return gVal ? make_pair(lv, rv) : make_pair(rv, rv);
        if (lVal)
            return make_pair(lv, rv);
        pushdown();
        const auto l = lc->find();
        if (!empty(l) && l.second < mv)
            return l;
        const auto r = rc->find();
        if (empty(l) && empty(r))
            return make_pair(rv, rv);
        if (empty(l) && !empty(r))
            return r;
        if (r.first != l.second && !empty(l))
            return l;
        return make_pair(l.first, r.second);
    }

private:
    void push(const unsigned int laz)
    {
        this->lVal -= laz;
        this->gVal -= laz;
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
    unsigned int laz, lVal, gVal;
    unsigned int lv, mv, rv;
} SNodes[maxn * 4 + 1], *SRoot = SNodes;
unsigned int blocks[maxn];

int main()
{
    unsigned int n, ans = 0;
    cin >> n;
    for (unsigned int* i = blocks; i < blocks + n; i++)
    {
        unsigned int t;
        cin >> t >> *i;
    }
    {
        STree* c = SNodes;
        SRoot->build(0, n, c, blocks);
    }
    while (true)
    {
        const auto t = SRoot->find();
        if (t.second == t.first)
            break;
        SRoot->minus(t.first, t.second, SRoot->query(t.first, t.second));
        ++ans;
    }
    cout << ans << endl;
    return 0;
}