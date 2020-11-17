#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2e5, maxq = 2e5;
constexpr long long inf = LLONG_MAX;

char inbuf[maxn * 2 * 6 + maxq * (6 + 6 + 10) + 10000], *inpos = inbuf, outbuf[maxq * 19 + 1000], *outpos = outbuf;

template <class T>
T readInt()
{
    bool neg = false;
    T ret = 0;
    char c = *(inpos++);
    while (c != '-' && !isdigit(c))
        c = *(inpos++);
    if (c == '-')
    {
        neg = true;
        c = *(inpos++);
    }
    while (isdigit(c))
    {
        ret = ret * 10 + (c - '0');
        c = *(inpos++);
    }
    return neg ? -ret : ret;
}
template <class... Args>
inline void writeOut(const char* fmt, const Args... args)
{
    outpos += sprintf(outpos, fmt, args...);
}

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn + 10];
const Edge* head[maxn + 10];
class FTree
{
public:
    void init(const unsigned int n);
    void add(unsigned int pos, const int delt);
    long long sum(const unsigned int l, const unsigned int r) const;

private:
    long long prefixSum(unsigned int p) const;

    unsigned int size;
    long long val[maxn + 10];
};
unsigned int a[maxn + 10];
long long w[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
}

#define lowbit(x) ((x) & -(x))
void FTree::init(const unsigned int n)
{
    size = n;
    memset(val, 0, sizeof(val));
}
void FTree::add(unsigned int pos, const int delt)
{
    for (; pos <= size; pos += lowbit(pos))
        val[pos] += delt;
}
long long FTree::sum(const unsigned int l, const unsigned int r) const
{
    return prefixSum(r - 1) - prefixSum(l - 1);
}
long long FTree::prefixSum(unsigned int p) const
{
    long long ret = 0;
    for (; p; p -= lowbit(p))
        ret += val[p];
    return ret;
}
#undef lowbit

namespace Chain
{
    enum
    {
        Sum = 0,
        Tag = 1
    };
    FTree st[2];
    unsigned int son[maxn + 10], fa[maxn + 10], top[maxn + 10], dep[maxn + 10], dfn[maxn + 10];
    long long tagv[maxn + 10];

    unsigned int dfsSon(const unsigned int x, const unsigned int f)
    {
        dep[x] = dep[f] + 1;
        fa[x] = f;
        unsigned int ret = 1, ms = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            const unsigned int sz = dfsSon(to, x);
            ret += sz;
            if (sz > ms)
            {
                ms = sz;
                son[x] = to;
            }
        }
        return ret;
    }
    void dfsChain(const unsigned int x, const unsigned int tp)
    {
        static unsigned int sta = 0;
        top[x] = tp;
        dfn[x] = ++sta;
        if (son[x])
            dfsChain(son[x], tp);
        for (const Edge* i = head[x]; i; i = i->pre)
            if (!dfn[i->to])
                dfsChain(i->to, i->to);
    }
    static void init(const unsigned int n)
    {
        dfsSon(0, 0);
        dfsChain(0, 0);
        st[Tag].init(n + 1);
        st[Sum].init(n + 1);
        for (unsigned int i = 0; i <= n; ++i)
            st[Sum].add(dfn[i], w[i]);
    }
    void update(const unsigned int x, const long long delt)
    {
        st[Sum].add(dfn[x], delt);
        st[Tag].add(dfn[x], delt);
        tagv[x] += delt;
    }

    unsigned int lca(unsigned int a, unsigned int b)
    {
        while (top[a] != top[b])
        {
            if (dep[top[a]] > dep[top[b]])
                swap(a, b);
            b = fa[top[b]];
        }
        return dep[a] < dep[b] ? a : b;
    }
    template <unsigned int sel>
    long long pathSum(unsigned int a, unsigned int b)
    {
        long long ret = 0;
        while (top[a] != top[b])
        {
            if (dep[top[a]] > dep[top[b]])
                swap(a, b);
            ret += st[sel].sum(dfn[top[b]], dfn[b] + 1);
            b = fa[top[b]];
        }
        if (dep[a] > dep[b])
            swap(a, b);
        ret += st[sel].sum(dfn[a], dfn[b] + 1);
        return ret;
    }
    inline long long vertexVal(unsigned int x)
    {
        return tagv[fa[x]] + tagv[x] + w[x];
    }
    inline long long pathSum(unsigned int a, unsigned int b, const unsigned int lc)
    {
        if (a == b)
            return vertexVal(a);
        long long ret = pathSum<Sum>(a, b) + tagv[fa[lc]];
        if (a != lc && b != lc)
            ret += pathSum<Tag>(fa[a], fa[b]) + tagv[lc];
        else if (a == lc)
            ret += pathSum<Tag>(a, fa[b]);
        else
            ret += pathSum<Tag>(b, fa[a]);
        return ret;
    }
}
static void buildGraph(const unsigned int n)
{
    static unsigned int stk[maxn + 10];
    unsigned int* top = stk;
    for (unsigned int i = n; i; --i)
    {
        while (top > stk && a[*(top - 1)] <= a[i])
            --top;
        addEdge(top > stk ? *(top - 1) : 0, i);
        *(top++) = i;
    }
}
long long sum(const unsigned int x, const unsigned int y)
{
    const unsigned int lc = Chain::lca(x, y);
    const unsigned int z = lc == x || lc == y ? Chain::fa[lc] : lc;
    if (!z)
        return inf;
    else
        return Chain::pathSum(x, y, lc) + (z != lc ? Chain::vertexVal(z) : 0);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin.read(inbuf, sizeof(inbuf));
    const unsigned int n = readInt<unsigned int>(), q = readInt<unsigned int>();
    generate_n(a + 1, n, readInt<unsigned int>);
    generate_n(w + 1, n, readInt<int>);
    buildGraph(n);
    Chain::init(n);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int typ = readInt<unsigned int>();
        if (typ == 1)
        {
            const unsigned int x = readInt<unsigned int>();
            const long long v = readInt<long long>();
            Chain::update(x, v);
        }
        else
        {
            const unsigned int x = readInt<unsigned int>(), y = readInt<unsigned int>();
            const long long v = sum(x, y);
            if (v == inf)
                writeOut("?\n");
            else
                writeOut("%lld\n", v);
        }
    }
    cout.write(outbuf, outpos - outbuf);
    return 0;
}