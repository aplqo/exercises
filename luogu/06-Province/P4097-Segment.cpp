#ifdef APTEST
#include "debug_tools/program.h"
#endif
#if !defined APTEST && !defined APDEBUG
#define NDEBUG
#endif
#include <algorithm>
#include <cassert>
#include <iostream>
using namespace std;
constexpr unsigned int modx = 39989, mody = 1e9;
const unsigned long long maxn = 1e5, maxx = 39989;
using real = long double;
constexpr real eps = 1e-8;

struct Line
{
    Line() = default;
    Line(int x1, long long y1, int x2, long long y2)
        : l(x1)
        , r(x2)
        , typ(x1 == x2)
    {
        if (typ)
            p = y2;
        else
        {
            k = (real(y1) - y2) / (real(x1) - x2);
            b = y1 - k * x1;
        }
    }
    inline real eval(const int x) const
    {
        return typ ? p : k * x + b;
    }
    inline real intersect(const Line& r) const
    {
        return typ ? p : (r.b - this->b) / (this->k - r.k);
    }

    bool typ;
    real k, b;
    unsigned int p;
    unsigned int l, r;
} lin[maxn + 10];
inline bool equal(const real a, const real b)
{
    return abs(a - b) <= eps;
}
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur))
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        val = 0;
        if (r - l == 1)
            return;
        lc = ++cur;
        rc = ++cur;
        lc->build(l, mv, cur);
        rc->build(mv, r, cur);
    }
    unsigned int find(const unsigned int x) const
    {
        if (rv - lv == 1)
            return val;
        const unsigned int t = x < mv ? lc->find(x) : rc->find(x);
        const real tv = lin[t].eval(x), cv = lin[val].eval(x);
        if (equal(tv, cv))
            return min(t, val);
        else
            return tv > cv ? t : val;
    }
    void modify(const unsigned int l, const unsigned int r, const unsigned int line)
    {
        if (l == lv && r == rv)
        {
            add(line);
            return;
        }
        if (r <= mv)
            lc->modify(l, r, line);
        else if (l >= mv)
            rc->modify(l, r, line);
        else
        {
            lc->modify(l, mv, line);
            rc->modify(mv, r, line);
        }
    }

private:
    void add(const unsigned int ln)
    {
        const Line cur = lin[val], nxt = lin[ln];
        const real cvl = cur.eval(lv), cvr = cur.eval(rv - 1), nvl = nxt.eval(lv), nvr = nxt.eval(rv - 1);
        {
            const bool el = equal(cvl, nvl), er = equal(cvr, nvr);
            if (el && er)
            {
                val = min(val, ln);
                return;
            }
            if (cvl >= nvl && cvr >= nvr)
            {
                addChild(ln, el, er);
                return;
            }
            if (cvl <= nvl && cvr <= nvr)
            {
                addChild(val, el, er);
                val = ln;
                return;
            }
        }

        assert(rv - lv > 1);
        const real i = cur.intersect(nxt), mid = (lv + rv - 1) / 2.0;
        if (cvl >= nvl)
        {
            if (i < mid)
            {
                lc->add(val);
                this->val = ln;
            }
            else
                rc->add(ln);
        }
        else
        {
            if (i < mid)
                lc->add(ln);
            else
            {
                rc->add(val);
                this->val = ln;
            }
        }
    }
    void addChild(const unsigned int t, const bool l, const bool r)
    {
        if (rv - lv == 1)
            return;
        if (l)
            lc->add(t);
        if (r)
            rc->add(t);
    }

    STree *lc, *rc;
    unsigned int lv, mv, rv;
    unsigned int val;
} SNodes[maxn * 4 + 1], *SRoot = SNodes;

void addLine(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1)
{
    static unsigned int cur = 1;
    if (x0 > x1)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    new (&lin[cur]) Line(x0, y0, x1, y1);
    SRoot->modify(lin[cur].l, lin[cur].r + 1, cur);
    ++cur;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned long long n, lst = 0;
    cin >> n;
    {
        STree* c = SNodes;
        SRoot->build(0, maxx + 1, c);
    }
    const static auto decodex = [&lst](const unsigned int x) -> unsigned int {
#ifndef NoMod
        return (1ll * x + lst + modx - 1) % modx + 1;
#else
        return x;
#endif
    };
    const static auto decodey = [&lst](const unsigned int y) -> unsigned int {
#ifndef NoMod
        return (1ll * y + lst + mody - 1) % mody + 1;
#else
        return y;
#endif
    };
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int op;
        cin >> op;
        if (op == 0)
        {
            unsigned int x;
            cin >> x;
            cout << (lst = SRoot->find(decodex(x))) << endl;
        }
        else
        {
            unsigned int x0, y0, x1, y1;
            cin >> x0 >> y0 >> x1 >> y1;
            addLine(decodex(x0), decodey(y0), decodex(x1), decodey(y1));
        }
    }
    return 0;
}