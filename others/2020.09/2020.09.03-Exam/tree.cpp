#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace std;
const unsigned int maxn = 300000, maxm = 300000;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 1];
int cnt[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur++;
}
namespace lca
{
    unsigned int top[maxn + 1], son[maxn + 1], fa[maxn + 1], dep[maxn + 1];

    unsigned int dfsSize(const unsigned int x, const unsigned int f)
    {
        fa[x] = f;
        dep[x] = dep[f] + 1;
        unsigned int ret = 1, ms = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (to == f)
                continue;
            const unsigned int sz = dfsSize(to, x);
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
    static void init()
    {
        dfsSize(1, 0);
        dfsChain(1, 1);
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
}
void dfsCnt(const unsigned int x, const unsigned int f)
{
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dfsCnt(to, x);
        cnt[x] += cnt[to];
    }
}
static void addNonTree(istream& in, const unsigned int m)
{
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        in >> x >> y;
        ++cnt[x];
        ++cnt[y];
        cnt[lca::lca(x, y)] -= 2;
    }
    dfsCnt(1, 0);
}
unsigned long long count(const unsigned int n, const unsigned int m)
{
    unsigned long long ret = 0;
    for (unsigned int i = 2; i <= n; ++i)
    {
        if (!cnt[i])
            ret += m;
        else if (cnt[i] == 1)
            ++ret;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
#if !defined APTEST && !defined APDEBUG
    ifstream in("tree.in");
    ofstream out("tree.out");
#else
    istream& in = cin;
    ostream& out = cout;
#endif
    unsigned int n, m;
    in >> n >> m;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        in >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    lca::init();
    addNonTree(in, m);
    out << count(n, m) << "\n";
    return 0;
}