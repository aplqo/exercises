#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 100000;
constexpr unsigned int inf = UINT_MAX;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2];
const Edge* head[maxn + 1];
class STree
{
public:
    void build(const unsigned int l, const unsigned int r, STree*(&cur), const unsigned int dat[]);
    void replace(const unsigned int l, const unsigned int r, const unsigned int v);
    [[nodiscard]] unsigned int query(const unsigned int l, const unsigned int r) const;

private:
    void push(const unsigned int l);
    void pushdown();

    unsigned int lv, mv, rv;
    unsigned int val, laz;
    STree *lc, *rc;
};
unsigned int siz[maxn + 1], dep[maxn + 1];
unsigned int val[maxn + 1], n;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}

void STree::build(const unsigned int l, const unsigned int r, STree*(&cur), const unsigned int dat[])
{
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    laz = 0;
    if (r - l == 1)
    {
        val = dat[l];
        return;
    }
    lc = ++cur;
    lc->build(l, mv, cur, dat);
    rc = ++cur;
    rc->build(mv, r, cur, dat);
    this->val = min(lc->val, rc->val);
}
void STree::replace(const unsigned int l, const unsigned int r, const unsigned int v)
{
    if (l == lv && r == rv)
    {
        push(v);
        return;
    }
    pushdown();
    if (r <= mv)
        lc->replace(l, r, v);
    else if (l >= mv)
        rc->replace(l, r, v);
    else
    {
        lc->replace(l, mv, v);
        rc->replace(mv, r, v);
    }
    this->val = min(lc->val, rc->val);
}
[[nodiscard]] unsigned int STree::query(const unsigned int l, const unsigned int r) const
{
    if (laz || (l == lv && r == rv))
        return val;
    if (r <= mv)
        return lc->query(l, r);
    else if (l >= mv)
        return rc->query(l, r);
    else
        return min(lc->query(l, mv), rc->query(mv, r));
}
void STree::push(const unsigned int l)
{
    val = laz = l;
}
void STree::pushdown()
{
    if (!laz)
        return;
    lc->push(laz);
    rc->push(laz);
    laz = 0;
}
namespace Sub
{
    unsigned int discover[maxn + 10], finish[maxn + 10];
    vector<unsigned int> chl[maxn + 10];

    inline bool isSubtree(const unsigned int x, const unsigned int f)
    {
        return discover[f] < discover[x] && finish[x] < finish[f];
    }
    unsigned int findSubtree(const unsigned int x, const unsigned int rt)
    {
        unsigned int l = 0, r = chl[rt].size() - 1;
        while (l != r)
        {
            const unsigned int mid = (l + r) >> 1, mp = chl[rt][mid];
            if (finish[mp] < discover[x])
                l = mid + 1;
            else if (discover[mp] > finish[x])
                r = mid - 1;
            if (x == mp || (discover[mp] < discover[x] && finish[x] < finish[mp]))
                return mp;
        }
        return chl[rt][l];
    }
}
namespace Chain
{
    unsigned int dfn[maxn + 1], fa[maxn + 1], top[maxn + 1], son[maxn + 1];
    STree SNodes[maxn * 4 + 10], *root = SNodes;

    void dfsSon(const unsigned int x, const unsigned int f)
    {
        dep[x] = dep[f] + 1;
        fa[x] = f;
        siz[x] = 1;
        unsigned int ms = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (to == f)
                continue;
            dfsSon(to, x);
            siz[x] += siz[to];
            if (siz[to] > ms)
            {
                ms = siz[to];
                son[x] = to;
            }
        }
    }
    void dfsChain(const unsigned int x, const unsigned int tp)
    {
        static unsigned int sta = 0, sfsta = 0;
        top[x] = tp;
        dfn[x] = ++sta;
        Sub::discover[x] = ++sfsta;
        if (son[x])
        {
            dfsChain(son[x], tp);
            Sub::chl[x].push_back(son[x]);
        }
        for (const Edge* i = head[x]; i; i = i->pre)
            if (!dfn[i->to])
            {
                Sub::chl[x].push_back(i->to);
                dfsChain(i->to, i->to);
            }
        Sub::finish[x] = ++sfsta;
    }
    void replace(unsigned int x, unsigned int y, const unsigned int v)
    {
        while (top[x] != top[y])
        {
            if (dep[top[x]] > dep[top[y]])
                swap(x, y);
            root->replace(dfn[top[y]], dfn[y] + 1, v);
            y = fa[top[y]];
        }
        if (dfn[x] > dfn[y])
            swap(x, y);
        root->replace(dfn[x], dfn[y] + 1, v);
    }
    static void init(const unsigned int n)
    {
        static unsigned int tmp[maxn + 1];
        dfsSon(1, 0);
        dfsChain(1, 1);
        for (unsigned int i = 1; i <= n; ++i)
            tmp[dfn[i]] = val[i];
        STree* c = SNodes;
        root->build(1, n + 1, c, tmp);
    }
    inline unsigned int querySubtree(const unsigned int x)
    {
        return root->query(dfn[x], dfn[x] + siz[x]);
    }
    inline unsigned int queryExclude(const unsigned int x)
    {
        unsigned int ret = inf;
        if (dfn[x] != 1)
            ret = root->query(1, dfn[x]);
        if (dfn[x] + siz[x] < n + 1)
            ret = min(ret, root->query(dfn[x] + siz[x], n + 1));
        return ret;
    }
}
using Chain::replace;
unsigned int query(const unsigned int root, const unsigned int x)
{
    if (x == root)
        return Chain::querySubtree(1);
    else if (!Sub::isSubtree(root, x))
        return Chain::querySubtree(x);
    else
        return Chain::queryExclude(Sub::findSubtree(root, x));
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int m, root;
    cin >> n >> m;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        addEdge(x, y);
    }
    copy_n(istream_iterator<unsigned int>(cin), n, val + 1);
    Chain::init(n);
    cin >> root;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int op;
        cin >> op;
        switch (op)
        {
        case 1:
            cin >> root;
            break;
        case 2:
        {
            unsigned int x, y, v;
            cin >> x >> y >> v;
            replace(x, y, v);
            break;
        }
        case 3:
        {
            unsigned int x;
            cin >> x;
            cout << query(root, x) << "\n";
            break;
        }
        }
    }
    return 0;
}