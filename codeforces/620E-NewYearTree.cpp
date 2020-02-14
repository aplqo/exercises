#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdint>
#include <iostream>
using namespace std;
const int maxn = 4e5;
using ans_t = uint_fast64_t;

struct vertex;
struct edge
{
    vertex* to;
    edge* pre = nullptr;
} ed[(maxn + 10) * 2];
struct vertex
{
    edge* head = nullptr;
    ans_t col;
    unsigned int siz = 0, pos = 0;
} v[maxn + 10];
unsigned int seq[maxn + 10];

inline unsigned int popcount(ans_t a)
{
    unsigned int ret = 0;
    for (ans_t i = 1; a; i <<= 1)
        if (a & i)
        {
            ++ret;
            a ^= i;
        }
    return ret;
}
inline void AddEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = v + to;
    cur->pre = v[from].head;
    v[from].head = cur;
    ++cur;
}
class tree
{
public:
    void build(const unsigned int l, const unsigned int r, const unsigned int dat[], tree*(&nxt))
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        if (r - l == 1)
        {
            val = v[dat[l]].col;
            return;
        }
        lc = ++nxt;
        lc->build(lv, mv, dat, nxt);
        rc = ++nxt;
        rc->build(mv, rv, dat, nxt);
        val = lc->val | rc->val;
    }
    void assign(const unsigned int l, const unsigned int r, const ans_t val)
    {
        if (l == lv && r == rv)
        {
            this->push(val);
            return;
        }
        if (rv - lv == 1)
        {
            this->val = val;
            return;
        }
        if (laz)
        {
            lc->push(laz);
            rc->push(laz);
            laz = 0;
        }
        if (r <= mv)
            lc->assign(l, r, val);
        else if (l >= mv)
            rc->assign(l, r, val);
        else
        {
            lc->assign(l, mv, val);
            rc->assign(mv, r, val);
        }
        this->val = lc->val | rc->val;
    }
    ans_t query(const unsigned int l, const unsigned int r)
    {
        if ((l == lv && r == rv) || rv - lv == 1)
            return val;
        if (laz)
        {
            lc->push(laz);
            rc->push(laz);
            laz = 0;
        }
        if (r <= mv)
            return lc->query(l, r);
        else if (l >= mv)
            return rc->query(l, r);
        else
            return lc->query(l, mv) | rc->query(mv, r);
    }

private:
    inline void push(ans_t laz)
    {
        this->val = laz;
        this->laz = laz;
    }
    tree *lc, *rc;
    unsigned int lv, rv, mv;
    ans_t val, laz = 0;
} tr[(maxn + 10) * 4], *tre = tr;
void dfs(vertex* const x)
{
    static unsigned int cur = 0;
    seq[cur] = x - v;
    x->pos = cur;
    x->siz = 1;
    ++cur;
    for (edge* i = x->head; i; i = i->pre)
        if (!(i->to->siz))
        {
            dfs(i->to);
            x->siz += i->to->siz;
        }
}

int main()
{
    ios_base::sync_with_stdio(false);
    unsigned int n, m;
    cin >> n >> m;
    for (vertex* i = v + 1; i < v + 1 + n; ++i)
    {
        unsigned int t;
        cin >> t;
        i->col = ans_t(1) << (t - 1);
    }
    for (unsigned int i = 0; i < n - 1; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        AddEdge(x, y);
        AddEdge(y, x);
    }
    dfs(v + 1);
    {
        tree* t = tr;
        tre->build(0, n, seq, t);
    }

    for (unsigned int i = 0; i < m; ++i)
    {
        char t;
        unsigned int ver, c;
        cin >> t >> ver;
        const vertex& thi = v[ver];
        switch (t)
        {
        case '1':
            cin >> c;
            tre->assign(thi.pos, thi.pos + thi.siz, ans_t(1) << (c - 1));
            break;
        case '2':
            cout << popcount(tre->query(thi.pos, thi.pos + thi.siz)) << endl;
            break;
        }
    }
    return 0;
}