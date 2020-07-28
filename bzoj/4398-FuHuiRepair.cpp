#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
using namespace std;
using dist_t = unsigned int;
const unsigned int maxn = 40000, maxm = 100000;
constexpr dist_t inf = numeric_limits<dist_t>::max();

struct edge
{
    unsigned int to;
    dist_t val;
    mutable bool enable = true;
    const edge *pre, *rev;
} ed[maxm * 2 + 1];
const edge* head[maxn + 1];

edge* addEdge(const unsigned int from, const unsigned int to, const dist_t val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    return cur++;
}
dist_t dijkstra(const unsigned int n)
{
    static bool vis[maxn + 1];
    static dist_t dis[maxn + 1];
    using pair_t = pair<dist_t, unsigned int>;
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    fill(dis, dis + 1 + n, inf);
    fill(vis, vis + 1 + n, false);
    for (const edge* i = head[1]; i; i = i->pre)
        if (i->enable)
        {
            dis[i->to] = i->val;
            q.emplace(i->val, i->to);
        }
    while (!q.empty())
    {
        const auto cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (const edge* i = head[cur.second]; i; i = i->pre)
            if (i->enable && dis[i->to] > cur.first + i->val)
            {
                dis[i->to] = cur.first + i->val;
                q.emplace(dis[i->to], i->to);
            }
    }
    return dis[1];
}
void enableEdge(const unsigned int pos, const bool typ)
{
    for (const edge* i = head[1]; i; i = i->pre)
    {
        const bool inv = !((i->to) & pos);
        i->enable = inv ^ typ;
        i->rev->enable = (!inv) ^ typ;
    }
}
dist_t solve(const unsigned int n)
{
    dist_t ret = inf;
    for (unsigned int i = 1; i <= n; i <<= 1)
    {
        enableEdge(i, true);
        ret = min(ret, dijkstra(n));
        enableEdge(i, false);
        ret = min(ret, dijkstra(n));
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
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int s, t, v, w;
        cin >> s >> t >> v >> w;
        edge *a = addEdge(s, t, v), *b = addEdge(t, s, w);
        a->rev = b;
        b->rev = a;
    }
    cout << solve(n) << endl;
    return 0;
}