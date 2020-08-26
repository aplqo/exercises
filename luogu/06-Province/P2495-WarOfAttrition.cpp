#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2.5e5, maxl = 20, maxe = 5e5;
constexpr unsigned long long inf = LLONG_MAX / 2;

template <class T, unsigned int siz>
class Array
{
public:
    void push_back(const T i) { *(cur++) = i; }
    T* begin() { return dat; }
    T* end() { return cur; }
    void clear() { cur = dat; }

private:
    T dat[siz] = {}, *cur = dat;
};
class STTable
{
public:
    void init(const unsigned int n, const unsigned int val[]);
    unsigned int query(const unsigned int l, const unsigned int r) const;
    static void getLog(const unsigned int n);

private:
    inline static bool comp(const unsigned int a, const unsigned int b);
    static unsigned int lg[maxe * 2 + 1];
    unsigned int tab[maxe * 2 + 1][maxl + 1];
} st;
struct Tree
{
    struct Edge
    {
        unsigned int to, val;
        const Edge* pre;
    } ed[maxe * 2 + 1], *cur = ed;
    const Edge* head[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int val);
} orig;
class VirtTree
{
    bool dirty[maxn + 1] = {};
    Array<unsigned int, maxn + 1> undoStk;

public:
    struct Edge
    {
        unsigned int to;
        const Edge* pre;
    } ed[maxe * 2 + 1], *cur = ed;
    const Edge* head[maxn + 1] = {};

    inline void addEdge(const unsigned int from, const unsigned int to);
    void build(const unsigned int k, unsigned int v[]);
    inline void clear();
} virt;
Array<unsigned int, maxe * 2 + 1> dfsSeq;
unsigned int STTable::lg[maxe * 2 + 1];
unsigned int dfn[maxn + 1];
bool key[maxn + 1];
unsigned long long f[maxn + 1], mine[maxn + 1];

void STTable::init(const unsigned int n, const unsigned int val[])
{
    for (unsigned int i = 0; i < n; ++i)
        tab[i][0] = val[i];
    for (unsigned int i = 1; i <= lg[n]; ++i)
        for (unsigned int j = 0; j + (1 << i) <= n; ++j)
            tab[j][i] = min(tab[j][i - 1], tab[j + (1 << (i - 1))][i - 1], comp);
}
void STTable::getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
inline unsigned int STTable::query(const unsigned int l, const unsigned int r) const
{
    const unsigned int d = lg[r + 1 - l];
    return min(tab[l][d], tab[r + 1 - (1 << d)][d], comp);
}
inline bool STTable::comp(const unsigned int a, const unsigned int b)
{
    return dfn[a] < dfn[b];
}

inline void Tree::addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    new (cur) Edge { to, val, head[from] };
    head[from] = cur++;
}
inline void VirtTree::addEdge(const unsigned int from, const unsigned int to)
{
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    if (!dirty[from])
    {
        dirty[from] = true;
        undoStk.push_back(from);
    }
}
void VirtTree::build(const unsigned int k, unsigned int v[])
{
    static unsigned int buildStk[maxn + 1];
    sort(v, v + k, [](const unsigned int a, const unsigned int b) { return dfn[a] < dfn[b]; });
    unsigned int* top = buildStk;
    *(top++) = 1;
    for (unsigned int* i = v + (v[0] == 1); i < v + k; ++i)
    {
        const unsigned int lca = st.query(dfn[*(top - 1)], dfn[*i]);
        while (top - buildStk > 1 && dfn[*(top - 2)] > dfn[lca])
        {
            addEdge(*(top - 2), *(top - 1));
            --top;
        }
        if (dfn[lca] < dfn[*(top - 1)])
        {
            addEdge(lca, *(top - 1));
            --top;
        }
        if (*(top - 1) != lca)
            *(top++) = lca;
        *(top++) = *i;
    }
    for (unsigned int* i = buildStk + 1; i < top; ++i)
        addEdge(*(i - 1), *i);
}
inline void VirtTree::clear()
{
    for (unsigned int i : undoStk)
    {
        head[i] = nullptr;
        dirty[i] = false;
    }
    cur = ed;
    undoStk.clear();
}
void dfsNo(const unsigned int x, const unsigned int f)
{
    static unsigned int sta = 0;
    dfn[x] = INT_MAX;
    for (auto i = orig.head[x]; i; i = i->pre)
    {
        if (i->to == f)
            continue;
        dfn[x] = min(dfn[x], sta++);
        dfsSeq.push_back(x);

        mine[i->to] = min<unsigned long long>(mine[x], i->val);
        dfsNo(i->to, x);
    }
    dfn[x] = min(dfn[x], sta++);
    dfsSeq.push_back(x);
}
void dfsVirt(const unsigned int x)
{
    f[x] = mine[x];
    if (key[x])
        return;
    bool hasChild = false;
    unsigned long long sum = 0;
    for (auto i = virt.head[x]; i; i = i->pre)
    {
        hasChild = true;
        dfsVirt(i->to);
        sum += f[i->to];
    }
    if (hasChild)
        f[x] = min(f[x], sum);
}
static void init()
{
    mine[1] = inf;
    dfsNo(1, 0);
    const unsigned int siz = dfsSeq.end() - dfsSeq.begin();
    STTable::getLog(siz);
    st.init(siz, dfsSeq.begin());
}
inline unsigned long long query(const unsigned int n, unsigned int qBuf[])
{
    virt.build(n, qBuf);
    for_each(qBuf, qBuf + n, [](unsigned int i) { key[i] = true; });
    dfsVirt(1);
    virt.clear();
    for_each(qBuf, qBuf + n, [](unsigned int i) { key[i] = false; });
    return f[1];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v, w;
        cin >> u >> v >> w;
        orig.addEdge(u, v, w);
        orig.addEdge(v, u, w);
    }
    init();
    cin >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        static unsigned int qBuf[maxn + 1];
        unsigned int cnt;
        cin >> cnt;
        copy_n(istream_iterator<unsigned int>(cin), cnt, qBuf);
        cout << query(cnt, qBuf) << "\n";
    }
    return 0;
}