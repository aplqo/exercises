#ifdef APTEST
#include "debug_tools/program.h"
#endif
#if !defined APTEST && !defined APDEBUG
#define NDEBUG
#endif
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>
using namespace std;
const unsigned int maxc = 100000;

unsigned int c;
struct vertex
{
    vertex() = default;
    vertex(const unsigned int r, const unsigned int c)
        : r(r)
        , c(c)
    {
    }
    unsigned int vertexNo() const
    {
        return r * (::c) + this->c;
    }

    unsigned int r, c;
};
inline istream& operator>>(istream& is, vertex& v)
{
    is >> v.r >> v.c;
    --v.r;
    return is;
}
namespace unf
{
    unsigned int fa[maxc * 2 + 10], siz[maxc * 2 + 10];
    stack<unsigned int> s;
    static void init(const unsigned int n)
    {
        fill(siz, siz + 1 + n, 1);
        iota(fa, fa + 1 + n, 0);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : getFather(fa[x]);
    }
    unsigned int getFather(const vertex x)
    {
        return getFather(x.vertexNo());
    }
    void merge(const unsigned int x, const unsigned int y)
    {
        unsigned int fx = getFather(x), fy = getFather(y);
        if (fx == fy)
            return;
        if (siz[fy] > siz[fx])
            swap(fx, fy);
        s.push(fy);
        fa[fy] = fx;
        siz[fx] += siz[fy];
    }
    void merge(const vertex a, const vertex b)
    {
        merge(a.vertexNo(), b.vertexNo());
    }
    void undo(const size_t v)
    {
        while (s.size() > v)
        {
            const unsigned int cur = s.top();
            s.pop();
            siz[fa[cur]] -= siz[cur];
            fa[cur] = cur;
        }
    }
    inline size_t stat()
    {
        return s.size();
    }
}
struct edge
{
    edge() = default;
    edge(const vertex a, const vertex b, const unsigned int be)
        : a(a)
        , b(b)
        , beg(be)
    {
    }
    unsigned int edgeNo() const
    {
        return a.r == b.r ? a.r * c + min(a.c, b.c) : 2 * c + a.c;
    }
    vertex a, b;
    unsigned int beg = maxc + 1;
} ed[maxc * 3 + 3];
struct query
{
    vertex a, b;
    bool ans, vis = false;
} qry[maxc + 3];
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur))
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        if (r - l == 1)
            return;
        lc = ++cur;
        lc->build(l, mv, cur);
        rc = ++cur;
        rc->build(mv, r, cur);
    }
    void modify(const unsigned int l, const unsigned int r, const unsigned int val)
    {
        if (l == lv && r == rv)
        {
            this->val.push_back(val);
            return;
        }
        if (r <= mv)
            lc->modify(l, r, val);
        else if (l >= mv)
            rc->modify(l, r, val);
        else
        {
            lc->modify(l, mv, val);
            rc->modify(mv, r, val);
        }
    }
    void dfs()
    {
        const size_t s = unf::stat();
        for (auto i : val)
            unf::merge(ed[i].a, ed[i].b);
        if (rv - lv == 1)
        {
            query& q = qry[lv];
            if (q.vis)
                q.ans = unf::getFather(q.a) == unf::getFather(q.b);
            unf::undo(s);
            return;
        }
        lc->dfs();
        rc->dfs();
        unf::undo(s);
    }

private:
    STree *lc, *rc;
    unsigned int lv, mv, rv;
    vector<unsigned int> val;
} SNodes[(maxc + 3) * 4 + 1], *SRoot = SNodes;

int main()
{
    cin >> c;
    unf::init(3 * c + 1);
    {
        STree* c = SNodes;
        SRoot->build(0, maxc + 2, c);
    }
    unsigned int lst = 0;
    while (true)
    {
        char op[5];
        vertex a, b;
        cin >> op >> a >> b;
        switch (op[0])
        {
        case 'O':
        {
            const edge e(a, b, lst);
            ed[e.edgeNo()] = e;
            break;
        }
        case 'C':
        {
            const edge e(a, b, lst);
            const unsigned int en = e.edgeNo();
            if (ed[en].beg != lst)
                SRoot->modify(ed[en].beg, lst, en);
            ed[en].beg = maxc + 1;
            break;
        }
        case 'A':
            qry[lst].a = a;
            qry[lst].vis = true;
            qry[lst++].b = b;
            break;
        case 'E':
            goto en;
        }
    }
en:;
    for (unsigned int i = 0; i < c * 3 + 3; ++i)
        if (ed[i].beg != maxc + 1 && ed[i].beg != lst)
            SRoot->modify(ed[i].beg, lst, i);
    SRoot->dfs();
    for (query* i = qry; i < qry + lst; ++i)
        cout << (i->ans ? "Y" : "N") << endl;
    return 0;
}
