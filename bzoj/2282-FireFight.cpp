#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;
using result = pair<unsigned int, unsigned int>;
const unsigned int maxn = 500000;

struct edge
{
    unsigned int to;
    unsigned int val;
    edge* pre = nullptr;
} ed[maxn * 2];
edge* head[maxn + 1];
unsigned int d[maxn + 1], nxt[maxn + 1], pre[maxn + 1];
unsigned int dist[maxn + 1];
bool vis[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
result dfsPath(const unsigned int x, const unsigned int f)
{
    result ret(x, 0);
    for (edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        const result v = dfsPath(to, x);
        if (v.second + i->val > ret.second)
        {
            ret = v;
            ret.second += i->val;
            dist[x] = i->val;
            nxt[x] = to;
        }
    }
    return ret;
}
unsigned int dfsDist(const unsigned int x, const unsigned int f)
{
    unsigned int ret = 0;
    for (edge* i = head[x]; i; i = i->pre)
        if (i->to != f && !vis[i->to])
            ret = max(ret, dfsDist(i->to, x) + i->val);
    return ret;
}
pair<unsigned int, unsigned int> rad(const unsigned int n)
{
    const result v1 = dfsPath(1, 0);
    fill(nxt, nxt + 1 + n, 0);
    fill(dist, dist + 1 + n, 0);
    const result v2 = dfsPath(v1.first, 0);
    return make_pair(v1.first, v2.first);
}
void init(const unsigned int p, const unsigned int n)
{
    vis[p] = true;
    for (unsigned int i = p; nxt[i]; i = nxt[i])
    {
        vis[nxt[i]] = true;
        dist[nxt[i]] += dist[i];
        pre[nxt[i]] = i;
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (vis[i])
            d[i] = dfsDist(i, 0);
}
unsigned int solve(const unsigned int n, const unsigned int s)
{
    const auto v = rad(n);
    init(v.first, n);
    const unsigned int md = *max_element(d, d + n + 1);
    unsigned int ret = dist[v.second];
    unsigned int r = v.first;
    for (unsigned int l = v.first; l; l = nxt[l])
        for (; r && dist[pre[r]] - dist[pre[l]] <= s; r = nxt[r])
            ret = min(ret, max({ md, dist[pre[l]], dist[v.second] - dist[pre[r]] }));
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, s;
    cin >> n >> s;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
        addEdge(v, u, w);
    }
    cout << solve(n, s) << endl;
    return 0;
}