#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxm = 5e5, maxl = 20;
const unsigned int maxe = maxm * 2, maxv = maxn + maxm;

unsigned int readInt()
{
    unsigned int ret = 0;
    char c = getchar();
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
        if (r - l == 1)
            return;
        const unsigned int mv = (l + r) >> 1;
        STree* tmp = create();
        tmp->build(l, mv);
        lc = tmp;
        tmp = create();
        tmp->build(mv, r);
        rc = tmp;
    }
    void increase(const STree* old, const unsigned int lv, const unsigned int rv, const unsigned int p)
    {
        const unsigned int mv = (lv + rv) >> 1;
        if (rv - lv == 1)
        {
            this->val = old->val + 1;
            return;
        }
        if (p < mv)
        {
            this->rc = old->rc;
            STree* c = create();
            c->increase(old->lc, lv, mv, p);
            lc = c;
        }
        else
        {
            this->lc = old->lc;
            STree* c = create();
            c->increase(old->rc, mv, rv, p);
            rc = c;
        }
        this->val = lc->val + rc->val;
    }
    unsigned int kth(const STree* old, const unsigned int lv, const unsigned int rv, const unsigned int k) const
    {
        if (rv - lv == 1)
            return lv;
        const unsigned int mv = (lv + rv) >> 1;
        const unsigned int lsum
            = this->lc->val - old->lc->val;
        return k <= lsum ? lc->kth(old->lc, lv, mv, k) : rc->kth(old->rc, mv, rv, k - lsum);
    }

    static STree* create()
    {
        STree* ret = ++cur;
        ret->val = 0;
        return ret;
    }
    static STree* cur;

private:
    STree *lc, *rc;
    unsigned int val;
} SNodes[maxn * 4 + maxn * maxl * 4];
STree* STree::cur = SNodes - 1;
namespace unf
{
    unsigned int fa[maxv + 1], root[maxv + 1], siz[maxv + 1];

    inline void init(const unsigned int n)
    {
        iota(fa, fa + 1 + n, 0);
        fill(siz, siz + 1 + n, 1);
        copy(fa, fa + 1 + n, root);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    unsigned int mergeRoot(unsigned int x, unsigned int y)
    {
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
        return x;
    }
}

struct VirtEdge
{
    unsigned int to;
    const VirtEdge* pre;
} ved[maxe * 2 + 1];
const VirtEdge* head[maxv + 1];
unsigned int leftPos[maxv + 1], siz[maxv + 1], length[maxv + 1] = { UINT_MAX };
unsigned int fa[maxv + 1][maxl + 1], dfn[maxn + 1], dep[maxv + 1], inv[maxn + 1];
unsigned int h[maxn + 1];
unsigned int orig[maxn + 1], lg[1 << maxl];
struct edge
{
    unsigned int from, to;
    unsigned int val;
} ed[maxm];
unsigned int vCnt, n;
const STree* root[maxn + 1];

void addVirtEdge(const unsigned int from, const unsigned int to)
{
    static VirtEdge* cur = ved;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
void discratize(const unsigned int n)
{
    copy(h + 1, h + 1 + n, orig);
    sort(orig, orig + n);
    unsigned int* const e = unique(orig, orig + n);
    for (unsigned int* i = h + 1; i < h + 1 + n; ++i)
        *i = lower_bound(orig, e, *i) - orig;
}
bool connect(const edge& v)
{
    const unsigned int fx = unf::getFather(v.from), fy = unf::getFather(v.to);
    if (fx == fy)
        return false;
    ++vCnt;
    length[vCnt] = v.val;
    addVirtEdge(vCnt, unf::root[fx]);
    addVirtEdge(vCnt, unf::root[fy]);
    unf::root[unf::mergeRoot(fx, fy)] = vCnt;
    return true;
}
void kruskal(const unsigned int n, const unsigned int m)
{
    sort(ed, ed + m, [](const edge& a, const edge& b) { return a.val < b.val; });
    edge* p = ed;
    for (unsigned int i = n; i > 1 && p < ed + m; --i, ++p)
        while (p < ed + m && !connect(*p))
            ++p;
}
void dfs(const unsigned int x, const unsigned int f)
{
    static unsigned int sta = 0;
    fa[x][0] = f;
    dep[x] = dep[f] + 1;
    for (unsigned int i = 1; i <= maxl; ++i)
        fa[x][i] = fa[fa[x][i - 1]][i - 1];
    bool first = true;
    if (x <= n)
    {
        siz[x] = 1;
        dfn[x] = ++sta;
        inv[sta] = x;
        leftPos[x] = x;
    }
    for (const VirtEdge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            dfs(i->to, x);
            siz[x] += siz[i->to];
            if (first)
                leftPos[x] = leftPos[i->to];
            first = false;
        }
}
static void init(const unsigned int n, const unsigned int m)
{
    unf::init(n);
    vCnt = n;
    kruskal(n, m);
    getLog(1 << maxl);
    dfs(vCnt, 0);
    discratize(n);
    {
        STree* c = STree::create();
        c->build(0, n);
        root[0] = c;
    }
    for (unsigned int i = 1; i <= n; ++i)
    {
        STree* t = STree::create();
        t->increase(root[i - 1], 0, n, h[inv[i]]);
        root[i] = t;
    }
}
unsigned int findRoot(unsigned int p, const unsigned int val)
{
    for (unsigned int i = maxl; i;)
        if (length[fa[p][i - 1]] > val)
            --i;
        else
            p = fa[p][i - 1];
    return p;
}
unsigned int query(const unsigned int v, const unsigned int x, const unsigned int k)
{
    const unsigned int p = findRoot(v, x);
    if (siz[p] < k)
        return UINT_MAX;
    return orig[root[dfn[leftPos[p]] + siz[p] - 1]->kth(root[dfn[leftPos[p]] - 1], 0, n, siz[p] + 1 - k)];
}

int main(int, char* argv[])
{
    n = readInt();
    const auto m = readInt();
    const auto q = readInt();
    generate(h + 1, h + 1 + n, readInt);
    for (edge* i = ed; i < ed + m; ++i)
    {
        i->from = readInt();
        i->to = readInt();
        i->val = readInt();
    }
    init(n, m);
    for (unsigned int i = 0; i < q; ++i)
    {
        const auto v = readInt();
        const auto x = readInt();
        const auto k = readInt();
        const unsigned int a = query(v, x, k);
        if (a == UINT_MAX)
            printf("-1\n");
        else
            printf("%u\n", a);
    }
    return 0;
}