#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5, maxm = 5e5;

char inbuf[maxn * 30 + 1000], *inpos = inbuf, outbuf[maxm * 20 + 1000], *outpos = outbuf;

template <class T = unsigned int>
T readUInt()
{
    T ret = 0;
    char c = *(inpos++);
    while (!isdigit(c))
        c = *(inpos++);
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = *(inpos++);
    }
    return ret;
}
void writeUInt(const unsigned int n, const unsigned int m)
{
    outpos += sprintf(outpos, "%u %u\n", n, m);
}

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int son[maxn + 10], dep[maxn + 10], fa[maxn + 10], top[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
    dep[x] = dep[f] + 1;
    fa[x] = f;
    unsigned int ret = 1, ms = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
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
    top[x] = tp;
    if (son[x])
        dfsChain(son[x], tp);
    for (const Edge* i = head[x]; i; i = i->pre)
        if (!top[i->to])
            dfsChain(i->to, i->to);
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
pair<unsigned int, unsigned int> findVertex(const unsigned int x, const unsigned int y, const unsigned int z)
{
    static const auto dist = [](const unsigned int x, const unsigned int y, const unsigned int l) {
        return dep[x] + dep[y] - 2 * dep[l];
    };
    const unsigned int l1 = lca(x, y), l2 = lca(y, z), l3 = lca(x, z);
    if (l2 == l1)
        return make_pair(dist(y, l3, l1) + dist(x, z, l3), l3);
    else if (l1 == l3)
        return make_pair(dist(x, l2, l1) + dist(y, z, l2), l2);
    else
        return make_pair(dist(z, l1, l2) + dist(x, y, l1), l1);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin.read(inbuf, sizeof(inbuf));
    const unsigned int n = readUInt(), m = readUInt();
    for (unsigned int i = 1; i < n; ++i)
    {
        const unsigned int a = readUInt(), b = readUInt();
        addEdge(a, b);
    }
    dfsSon(1, 0);
    dfsChain(1, 1);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int x = readUInt(), y = readUInt(), z = readUInt();
        const auto v = findVertex(x, y, z);
        writeUInt(v.second, v.first);
    }
    cout.write(outbuf, outpos - outbuf);
    return 0;
}