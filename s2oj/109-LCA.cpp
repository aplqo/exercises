#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5, maxq = 5e5;

char inbuf[maxn * 2 * 10 + maxq * 2 * 10 + 1000], *inpos = inbuf, outbuf[maxq * 11 + 100], *outpos = outbuf;

unsigned int readUInt()
{
    unsigned int ret = 0;
    char c = *(inpos++);
    while (!isdigit(c))
        c = *(inpos++);
    while (isdigit(c))
    {
        ret = ret * 10 + (c - '0');
        c = *(inpos++);
    }
    return ret;
}
void writeUInt(const unsigned int i)
{
    outpos += sprintf(outpos, "%u\n", i);
}

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int top[maxn + 10], dep[maxn + 10], son[maxn + 10], fa[maxn + 10];

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
    fa[x] = f;
    dep[x] = dep[f] + 1;
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
void dfsChain(unsigned int x, const unsigned int tp)
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

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin.read(inbuf, sizeof(inbuf));
    const unsigned int n = readUInt(), m = readUInt(), s = readUInt();
    for (unsigned int i = 1; i < n; ++i)
    {
        const unsigned int a = readUInt(), b = readUInt();
        addEdge(a, b);
    }
    dfsSon(s, 0);
    dfsChain(s, s);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int u = readUInt(), v = readUInt();
        writeUInt(lca(u, v));
    }
    cout.write(outbuf, outpos - outbuf);
    return 0;
}