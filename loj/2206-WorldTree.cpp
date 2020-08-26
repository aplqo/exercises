#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 300000, maxl = 19;
constexpr unsigned int inf = UINT_MAX;
using MPos = pair<unsigned int, unsigned int>;

inline MPos operator+(const MPos a, const unsigned int b)
{
    return MPos { a.first + b, a.second };
}
struct Tree
{
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[maxn * 2 + 1], *cur = ed;
    const Edge* head[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to);
} orig;
class VirtTree
{
public:
    struct Edge
    {
        unsigned int to, val;
        const Edge* pre;
    } ed[maxn + 1], *cur = ed;
    const Edge* head[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to);
    void build(const unsigned int n, unsigned int v[]);
} virt;
unsigned int key[maxn + 1], ans[maxn + 1], kcur;
MPos f[maxn + 1];
unsigned int dfn[maxn + 1], siz[maxn + 1], dep[maxn + 1];
unsigned int fa[maxn + 1][maxl + 1], lg[maxn + 1], sub[maxn + 1][maxl + 1];

static void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
void dfsNo(const unsigned int x, const unsigned int f)
{
    static unsigned int sta = 0;
    dfn[x] = ++sta;
    siz[x] = 1;
    for (auto i = orig.head[x]; i; i = i->pre)
        if (i->to != f)
        {
            dfsNo(i->to, x);
            siz[x] += siz[i->to];
        }
}
void dfsLca(const unsigned int x, const unsigned int f)
{
    fa[x][0] = f;
    dep[x] = dep[f] + 1;
    sub[x][0] = siz[x];
    for (unsigned int i = 1; i <= lg[dep[x]]; ++i)
    {
        fa[x][i] = fa[fa[x][i - 1]][i - 1];
        sub[x][i] = sub[x][i - 1] + sub[fa[x][i - 1]][i - 1];
    }
    for (auto i = orig.head[x]; i; i = i->pre)
        if (i->to != f)
            dfsLca(i->to, x);
}
unsigned int lca(unsigned int a, unsigned int b)
{
    if (dep[a] != dep[b])
    {
        if (dep[a] > dep[b])
            swap(a, b);
        for (unsigned int i = 0, t = dep[b] - dep[a]; t; t >>= 1, ++i)
            if (t & 0x01)
                b = fa[b][i];
    }
    if (a == b)
        return a;
    for (unsigned int i = lg[dep[a]] + 1; i;)
    {
        if (fa[a][i - 1] == fa[b][i - 1])
            --i;
        else
        {
            a = fa[a][i - 1];
            b = fa[b][i - 1];
        }
    }
    return fa[a][0];
}
inline unsigned int distance(const unsigned int x, const unsigned int fa)
{
    return dep[x] - dep[fa];
}
inline void Tree::addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}
inline void VirtTree::addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, dep[to] - dep[from], head[from] };
    head[from] = cur++;
}
void VirtTree::build(const unsigned int n, unsigned int v[])
{
    static unsigned int stk[maxn + 1];
    unsigned int* top = stk;
    sort(v, v + n, [](unsigned int a, unsigned int b) { return dfn[a] < dfn[b]; });
    *(top++) = 1;
    for (unsigned int* i = v + (v[0] == 1); i < v + n; ++i)
    {
        const unsigned int Lca = lca(*i, *(top - 1));
        while (top - stk > 1 && dfn[Lca] < dfn[*(top - 2)])
        {
            addEdge(*(top - 2), *(top - 1));
            --top;
        }
        if (dfn[Lca] < dfn[*(top - 1)])
        {
            addEdge(Lca, *(top - 1));
            --top;
        }
        if (Lca != *(top - 1))
            *(top++) = Lca;
        *(top++) = *i;
    }
    for (unsigned int* i = stk + 1; i < top; ++i)
        addEdge(*(i - 1), *i);
}
void dfsVirtChl(const unsigned int x)
{
    ans[x] = 0;
    f[x] = key[x] == kcur ? MPos { 0, x } : MPos { inf, inf };
    for (auto i = virt.head[x]; i; i = i->pre)
    {
        dfsVirtChl(i->to);
        f[x] = min(f[x], f[i->to] + i->val);
    }
}
unsigned int findPos(unsigned int x, const unsigned int father)
{
    const auto dist = [](unsigned int x, const unsigned int f, const unsigned int base) {
        return base + dep[x] - dep[f];
    };
    const unsigned int belx = f[x].second;
    unsigned int basx = f[x].first;
    for (unsigned int i = lg[dep[x] - dep[father]]; i;)
    {
        const unsigned int m = fa[x][i - 1];
        const unsigned int df = dist(m, father, f[father].first), dx = dist(x, m, basx);
        if (df < dx || (df == dx && f[father].second < belx))
            --i;
        else
        {
            basx = dx;
            x = m;
        }
    }
    return x;
}
unsigned int dfsVirtAns(const unsigned int x, const unsigned int father)
{
    unsigned int virtSiz = 0;
    for (auto i = virt.head[x]; i; i = i->pre)
    {
        f[i->to] = min(f[i->to], f[x] + i->val);
        virtSiz += dfsVirtAns(i->to, x);
    }
    virt.head[x] = nullptr;
    const unsigned int pos = f[x].second == f[father].second ? x : findPos(x, father);
    ans[f[x].second] += sub[pos][0] - virtSiz;
    return sub[pos][0];
}
void query(const unsigned int k, unsigned int qbuf[])
{
    ++kcur;
    virt.build(k, qbuf);
    for_each(qbuf, qbuf + k, [](unsigned int i) { key[i] = kcur; });
    dfsVirtChl(1);
    dfsVirtAns(1, 1);
    virt.cur = virt.ed;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    {
        unsigned int n;
        cin >> n;
        for (unsigned int i = 1; i < n; ++i)
        {
            unsigned int x, y;
            cin >> x >> y;
            orig.addEdge(x, y);
            orig.addEdge(y, x);
        }
        getLog(n);
        dfsNo(1, 0);
        dfsLca(1, 0);
    }
    {
        unsigned int q;
        cin >> q;
        for (unsigned int i = 0; i < q; ++i)
        {
            static unsigned int qbuf[maxn + 1], orig[maxn + 1];
            unsigned int k;
            cin >> k;
            copy_n(istream_iterator<unsigned int>(cin), k, qbuf);
            copy(qbuf, qbuf + k, orig);
            query(k, qbuf);
            transform(orig, orig + k, ostream_iterator<unsigned int>(cout, " "), [](unsigned int i) { return ans[i]; });
            cout << "\n";
        }
    }
    return 0;
}