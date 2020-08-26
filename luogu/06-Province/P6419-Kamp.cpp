#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
const unsigned int maxn = 5e5;

struct Edge
{
    unsigned int to;
    unsigned long long val;
    const Edge* pre;
} ed[maxn * 2 + 1];
const Edge* head[maxn + 1];
bool key[maxn + 1];
unsigned long long ans[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur++;
}
void dfsKey(const unsigned int x, const unsigned int f)
{
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            dfsKey(i->to, x);
            key[x] = key[x] || key[i->to];
        }
}
namespace lca
{
    unsigned int dep[maxn + 1], son[maxn + 1], top[maxn + 1], fa[maxn + 1];
    unsigned long long dist[maxn + 1];

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
            dist[to] = dist[x] + i->val;
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
    static void init(const unsigned int rt)
    {
        dfsSon(rt, 0);
        dfsChain(rt, rt);
    }
    unsigned int lca(unsigned int a, unsigned int b)
    {
        while (top[a] != top[b])
        {
            if (dep[top[a]] > dep[top[b]])
                a = fa[top[a]];
            else
                b = fa[top[b]];
        }
        return dep[a] > dep[b] ? b : a;
    }
    inline unsigned long long distance(const unsigned int a, const unsigned int b)
    {
        return dist[a] + dist[b] - 2 * dist[lca(a, b)];
    }
}
pair<unsigned long long, unsigned int> dfsPath(const unsigned int x, const unsigned int f)
{
    pair<unsigned long long, unsigned int> ret(0, x);
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        if (i->to == f || !key[i->to])
            continue;
        const auto r = dfsPath(i->to, x);
        if (r.first + i->val > ret.first)
        {
            ret = r;
            ret.first += i->val;
        }
    }
    return ret;
}
inline pair<unsigned int, unsigned int> rad(const unsigned int rt)
{
    const unsigned int v1 = dfsPath(rt, 0).second;
    return make_pair(v1, dfsPath(v1, 0).second);
}
void dfsNotKey(const unsigned int x, const unsigned int f)
{
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f && !key[i->to])
        {
            ans[i->to] = ans[x] + i->val;
            dfsNotKey(i->to, x);
        }
}
unsigned long long keyEdgeSum(const unsigned int n)
{
    unsigned long long ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        if (!key[i])
            continue;
        for (const Edge* j = head[i]; j; j = j->pre)
            if (key[j->to] && j->to < i)
                ret += j->val;
    }
    return ret;
}
void getans(const unsigned int n, const pair<unsigned int, unsigned int> r)
{
    const unsigned long long esum = 2 * keyEdgeSum(n);
    for (unsigned int i = 1; i <= n; ++i)
        if (key[i])
        {
            ans[i] = esum - max(lca::distance(i, r.first), lca::distance(i, r.second));
            dfsNotKey(i, 0);
        }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int x, y, z;
        cin >> x >> y >> z;
        addEdge(x, y, z);
        addEdge(y, x, z);
    }
    unsigned int rt = 0;
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int p;
        cin >> p;
        key[p] = true;
        rt = p;
    }
    lca::init(rt);
    dfsKey(rt, 0);
    getans(n, rad(rt));
    copy(ans + 1, ans + 1 + n, ostream_iterator<unsigned long long>(cout, "\n"));
    return 0;
}