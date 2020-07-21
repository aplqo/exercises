#ifdef APTEST
#include "debug_tools/program.h"
#endif
#if !defined APTEST && !defined APDEBUG
#define NDEBUG
#endif
#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 8e4, maxl = 17;

struct edge
{
    unsigned int to;
    const edge* pre;
} ed[maxn * 2 + 1];
const edge* head[maxn + 1];

unsigned int lg[1 << maxl];
unsigned int orig[maxn + 1], wight[maxn + 1];
unsigned int n;

class STree
{
public:
    void build(const unsigned int l, const unsigned int r)
    {
        if (r - l == 1)
            return;
        STree* tmp = create(l, mv);
        tmp->build(l, mv);
        lc = tmp;
        tmp = create(mv, r);
        tmp->build(mv, r);
        rc = tmp;
    }
    void increase(const STree* const old, const unsigned int p)
    {
        if (rv - lv == 1)
        {
            this->val = old->val + 1;
            return;
        }
        if (p < mv)
        {
            rc = old->rc;
            STree* tmp = create(lv, mv);
            tmp->increase(old->lc, p);
            lc = tmp;
        }
        else
        {
            lc = old->lc;
            STree* tmp = create(mv, rv);
            tmp->increase(old->rc, p);
            rc = tmp;
        }
        this->val = lc->val + rc->val;
    }
    unsigned int kth(const unsigned int k, const STree* another, const STree* lca, const STree* flca) const
    {
        if (rv - lv == 1)
            return lv;
        const unsigned int lsum = this->lc->val + another->lc->val - lca->lc->val - flca->lc->val;
        return k <= lsum ? lc->kth(k, another->lc, lca->lc, flca->lc)
                         : rc->kth(k - lsum, another->rc, lca->rc, flca->rc);
    }

    inline static STree* create(const unsigned int l, const unsigned int r)
    {
        STree* ret = ++cur;
        ret->init(l, r);
        return ret;
    }
    static STree* cur;

private:
    inline void init(const unsigned int l, const unsigned int r)
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        val = 0;
#if defined APTEST || defined APDEBUG
        lc = rc = nullptr;
#endif
    }

    unsigned int lv, mv, rv;
    unsigned int val;
    const STree *lc, *rc;
} SNodes[(maxn + maxn * 30) * 4];
const STree* root[maxn + 1];
STree* STree::cur = SNodes - 1;

namespace unf
{
    unsigned int fa[maxn + 1], siz[maxn + 1];
    inline static void init(const unsigned int n)
    {
        iota(fa, fa + n + 1, 0);
        fill(siz, siz + 1 + n, 1);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    inline void merge(unsigned int x, unsigned int y)
    {
        x = getFather(x);
        y = getFather(y);
        if (x == y)
            return;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
    }
    inline unsigned int size(const unsigned int x)
    {
        return siz[getFather(x)];
    }
}
namespace lca
{
    unsigned int fa[maxn + 1][maxl + 1], dep[maxn + 1];

    inline void init(const unsigned int x, const unsigned int f)
    {
        fa[x][0] = f;
        dep[x] = dep[f] + 1;
        for (unsigned int i = 1; i <= maxl; ++i)
            fa[x][i] = fa[fa[x][i - 1]][i - 1];
    }
    unsigned int lca(unsigned int x, unsigned int y)
    {
        if (dep[x] != dep[y])
        {
            if (dep[x] > dep[y])
                swap(x, y);
            for (unsigned int d = dep[y] - dep[x], t = 0; d; d >>= 1, ++t)
                if (d & 0x01)
                    y = fa[y][t];
        }
        if (x == y)
            return x;
        for (unsigned int i = maxl; i;)
        {
            if (fa[x][i - 1] != fa[y][i - 1])
            {
                x = fa[x][i - 1];
                y = fa[y][i - 1];
            }
            else
                --i;
        }
        return fa[x][0];
    }
}

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
static void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
static void discretize(const unsigned int n)
{
    copy(wight + 1, wight + n + 1, orig);
    sort(orig, orig + n);
    unsigned int* e = unique(orig, orig + n);
    for (unsigned int* i = wight + 1; i < wight + n + 1; ++i)
        *i = lower_bound(orig, e, *i) - orig;
}
static void init(const unsigned int n)
{
    getLog(n);
    discretize(n);
    unf::init(n + 1);
    {
        STree* c = STree::create(0, n);
        c->build(0, n);
        root[0] = c;
    }
}
void dfs(const unsigned int x, const unsigned int f)
{
    lca::init(x, f);
    {
        STree* c = STree::create(0, n);
        c->increase(root[f], wight[x]);
        root[x] = c;
    }
    for (const edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
            dfs(i->to, x);
}

inline void connect(unsigned int x, unsigned int y)
{
    if (unf::size(x) < unf::size(y))
        swap(x, y);
    unf::merge(x, y);
    addEdge(x, y);
    addEdge(y, x);
    dfs(y, x);
}

inline unsigned int query(unsigned int x, unsigned int y, const unsigned int k)
{
    const unsigned int l = lca::lca(x, y);
    return orig[root[x]->kth(k, root[y], root[l], root[lca::fa[l][0]])];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    {
        unsigned int ca;
        cin >> ca;
    }
    unsigned int m, t;
    cin >> n >> m >> t;
    copy_n(istream_iterator<unsigned int>(cin), n, wight + 1);
    init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        addEdge(x, y);
        addEdge(y, x);
        unf::merge(x, y);
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (!root[i])
            dfs(i, 0);
    unsigned int lst = 0;
    for (unsigned int i = 0; i < t; ++i)
    {
        char op;
        unsigned int x, y;
        cin >> op >> x >> y;
#ifndef NoEncode
        x ^= lst;
        y ^= lst;
#endif
        if (op == 'Q')
        {
            unsigned int k;
            cin >> k;
#ifndef NoEncode
            k ^= lst;
#endif
            cout << (lst = query(x, y, k)) << endl;
        }
        else
            connect(x, y);
    }
    return 0;
}