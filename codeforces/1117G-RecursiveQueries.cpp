#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e6, maxq = 1e6, maxl = 19;

char inbuf[maxn * 10 + maxq * 20 + 1000], *inpos = inbuf, outbuf[maxq * 10 + 1000], *outpos = outbuf;

inline char get()
{
    return *(inpos++);
}
inline unsigned int readUInt()
{
    char c = get();
    unsigned int ret = 0;
    while (!isdigit(c))
        c = get();
    while (isdigit(c))
    {
        ret = ret * 10 + c - '0';
        c = get();
    }
    return ret;
}
void write(const unsigned long long v)
{
    outpos += sprintf(outpos, "%lld ", v);
}

unsigned int lg[maxn + 1], fa[maxn + 1][maxl + 1], dep[maxn + 1];
unsigned int p[maxn + 1];
unsigned int lc[maxn + 1], rc[maxn + 1], siz[maxn + 1];
long long sumL[maxn + 1], sumR[maxn + 1];
int depL[maxn + 1], depR[maxn + 1];
long long f[maxn + 1];

static void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
static unsigned int build(const unsigned int n)
{
    static unsigned int stk[maxn + 1];
    unsigned int *top = stk, root = 1;
    *(top++) = 1;
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int* ptr = top;
        while (ptr > stk && p[*(ptr - 1)] < p[i])
            --ptr;
        if (ptr > stk)
            rc[*(ptr - 1)] = i;
        else
            root = i;
        if (ptr < top)
            lc[i] = *ptr;
        top = ptr;
        *(top++) = i;
    }
    return root;
}
void dfs(const unsigned int x, const unsigned int father)
{
    if (!x)
        return;
    dep[x] = dep[father] + 1;
    fa[x][0] = father;
    for (unsigned int i = 1; i <= lg[dep[x]]; ++i)
        fa[x][i] = fa[fa[x][i - 1]][i - 1];

    dfs(lc[x], x);
    dfs(rc[x], x);

    siz[x] = siz[lc[x]] + siz[rc[x]] + 1;
    f[x] = siz[x] + f[lc[x]] + f[rc[x]];
}
void dfsPreSum(const unsigned int x, const unsigned int ddl = 0, const unsigned int ddr = 0)
{
    const long long nsl = sumL[x] + f[rc[x]] + x + siz[rc[x]];
    const long long nsr = sumR[x] + f[lc[x]] - (x - siz[lc[x]]);
    depL[x] += ddl;
    depR[x] += ddr;
    if (lc[x])
    {
        sumL[lc[x]] = nsl;
        depL[lc[x]] = depL[x];
        sumR[lc[x]] = sumR[x];
        depR[lc[x]] = depR[x];
        dfsPreSum(lc[x], 1, 0);
    }
    if (rc[x])
    {
        sumL[rc[x]] = sumL[x];
        depL[rc[x]] = depL[x];
        sumR[rc[x]] = nsr;
        depR[rc[x]] = depR[x];
        dfsPreSum(rc[x], 0, 1);
    }
    sumL[x] = nsl;
    sumR[x] = nsr;
}
unsigned int lca(unsigned int a, unsigned int b)
{
    if (dep[a] != dep[b])
    {
        if (dep[a] > dep[b])
            swap(a, b);
        for (unsigned int i = 0, d = dep[b] - dep[a]; d; d >>= 1, ++i)
            if (d & 0x01)
                b = fa[b][i];
    }
    if (a == b)
        return a;
    for (unsigned int i = lg[dep[a]]; i;)
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
unsigned long long query(const int l, const int r)
{
    const unsigned int Lca = lca(l, r);
    return (r + 1 - l)
        + sumL[l] - sumL[Lca] - static_cast<long long>(depL[l] - depL[Lca]) * (l - 1)
        + sumR[r] - sumR[Lca] + static_cast<long long>(depR[r] - depR[Lca]) * (r + 1);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin.read(inbuf, sizeof(inbuf));
    const unsigned int n = readUInt(), q = readUInt();
    generate_n(p + 1, n, readUInt);

    const unsigned int root = build(n);
    getLog(n);
    dfs(root, 0);
    dfsPreSum(root);

    static unsigned int ls[maxq + 1];
    generate_n(ls, q, readUInt);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int r = readUInt();
        write(query(ls[i], r));
    }
    cout.write(outbuf, outpos - outbuf);
    cout.put('\n');
    return 0;
}