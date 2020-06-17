#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
using namespace std;
const unsigned int maxn = 1e5, maxz = 1e5;

template <class T = unsigned int>
T read()
{
    char c = getchar();
    T ret = 0;
    while (!isdigit(c))
        c = getchar();
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = getchar();
    }
    return ret;
}
class STree
{
public:
    void build(const unsigned int l, const unsigned int r)
    {
        lv = l;
        rv = r;
        mv = (l + r) >> 1;
        lc = rc = nullptr;
        val = 0;
    }
    void modify(const unsigned int p, const int val)
    {
        if (rv - lv == 1)
        {
            this->val += val;
            return;
        }
        const auto create = [](STree*(&t), const unsigned int l, const unsigned int r) {
            if (t)
                return t;
            t = ++cur;
            t->build(l, r);
            return t;
        };
        if (p < mv)
            create(lc, lv, mv)->modify(p, val);
        else
            create(rc, mv, rv)->modify(p, val);
        this->val = max(get(lc), get(rc));
    }
    unsigned int findMax() const
    {
        return val ? findImpl() : 0;
    }
    void merge(STree* const s)
    {
        if (!s)
            return;
        if (rv - lv == 1)
        {
            val += s->val;
            return;
        }
        const auto may = [](STree* a, STree* b) {
            if (!a)
                return b;
            a->merge(b);
            return a;
        };
        lc = may(lc, s->lc);
        rc = may(rc, s->rc);
        val = max(get(lc), get(rc));
    }

    static STree* cur;

private:
    unsigned int findImpl() const
    {
        if (rv - lv == 1)
            return lv;
        return get(lc) == val ? lc->findImpl() : rc->findImpl();
    }
    inline static unsigned int get(const STree* const s)
    {
        return s ? s->val : 0;
    }

    unsigned int lv, mv, rv;
    STree *lc, *rc;
    int val;
} SNodes[maxn * 100 + 1];
STree* STree::cur = SNodes;
STree* root[maxn + 1];

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
unsigned int top[maxn + 1], dep[maxn + 1];
unsigned int fa[maxn + 1], son[maxn + 1];
unsigned int maxV[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}

unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
    fa[x] = f;
    dep[x] = dep[f] + 1;
    unsigned int ret = 1, s = 0;
    for (edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        const unsigned int sz = dfsSon(to, x);
        ret += sz;
        if (sz > s)
        {
            s = sz;
            son[x] = to;
        }
    }
    return ret;
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
    top[x] = tp;
    if (son[x])
        dfsChain(son[x], tp);
    for (edge* i = head[x]; i; i = i->pre)
        if (!top[i->to])
            dfsChain(i->to, i->to);
}
unsigned int lca(unsigned int x, unsigned int y)
{
    while (top[x] != top[y])
    {
        if (dep[top[x]] > dep[top[y]])
            x = fa[top[x]];
        else
            y = fa[top[y]];
    }
    return dep[x] > dep[y] ? y : x;
}

void dfs(const unsigned int x, const unsigned int f)
{
    for (edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            dfs(i->to, x);
            root[x]->merge(root[i->to]);
        }
    maxV[x] = root[x]->findMax();
}

int main()
{
    const auto n = read();
    const auto m = read();
    for (unsigned int i = 1; i < n; ++i)
    {
        const auto a = read();
        const auto b = read();
        addEdge(a, b);
        addEdge(b, a);
    }
    dfsSon(1, 0);
    dfsChain(1, 1);

    for (unsigned int i = 0; i <= n; ++i)
    {
        root[i] = STree::cur++;
        root[i]->build(1, maxz + 1);
    }
    --STree::cur;

    for (unsigned int i = 0; i < m; ++i)
    {
        const auto x = read();
        const auto y = read();
        const auto z = read();
        const unsigned int l = lca(x, y);
        root[x]->modify(z, 1);
        root[y]->modify(z, 1);
        root[l]->modify(z, -1);
        root[fa[l]]->modify(z, -1);
    }
    dfs(1, 0);
    for (unsigned int i = 1; i <= n; ++i)
        printf("%d\n", maxV[i]);
    return 0;
}