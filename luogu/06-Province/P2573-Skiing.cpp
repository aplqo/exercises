#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e6;

struct edge
{
    unsigned int from, to;
    unsigned long long val;
    const edge* pre;
} ed[maxm * 2 + 1], *cur = ed;
const edge* head[maxn + 1];
bool vis[maxn + 1];
unsigned int h[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const unsigned long long val)
{
    cur->from = from;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
namespace dsu
{
    unsigned int fa[maxn + 1], siz[maxn + 1];

    static void init(const unsigned int x)
    {
        fill(siz, siz + 1 + x, 1);
        iota(fa, fa + 1 + x, 0);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    bool tryMerge(unsigned int x, unsigned int y)
    {
        x = getFather(x);
        y = getFather(y);
        if (x == y)
            return false;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
        return true;
    }
}
void dfs(const unsigned int x)
{
    vis[x] = true;
    for (const edge* i = head[x]; i; i = i->pre)
        if (!vis[i->to])
            dfs(i->to);
}
unsigned long long kruskal(const unsigned int n)
{
    static const edge* tmp[maxm * 2 + 1];
    transform(ed, cur, tmp, [](const edge& i) { return &i; });
    const auto ec = distance(ed, cur);
    sort(tmp, tmp + ec, [](const edge* a, const edge* b) {
        return h[a->to] != h[b->to] ? h[a->to] > h[b->to] : a->val < b->val;
    });
    unsigned int p = 0;
    unsigned long long ret = 0;
    for (unsigned int i = 1; i < n; ++i)
    {
        for (; p < ec; ++p)
        {
            const edge* e = tmp[p];
            if (!vis[e->from] || !vis[e->to])
                continue;
            if (dsu::tryMerge(e->from, e->to))
                break;
        }
        ret += tmp[p]->val;
        ++p;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<unsigned int>(cin), n, h + 1);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v, k;
        cin >> u >> v >> k;
        if (h[u] >= h[v])
            addEdge(u, v, k);
        if (h[v] >= h[u])
            addEdge(v, u, k);
    }
    dfs(1);
    dsu::init(n);
    const unsigned int cnt = count(vis + 1, vis + 1 + n, true);
    cout << cnt << " " << kruskal(cnt) << endl;
    return 0;
}