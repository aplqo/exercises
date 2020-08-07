#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 400, maxm = 15000;
constexpr int inf = INT_MAX;

struct Edge
{
    unsigned int from, to, id;
    int cap, cost;
    Edge* pre;
} ed[maxm * 2 + 1];
Edge* head[maxn + 1];
int dis[maxn + 1], h[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const int cap, const int cost)
{
    static Edge* cur = ed;
    cur->from = from;
    cur->to = to;
    cur->id = cur - ed;
    cur->cap = cap;
    cur->cost = cost;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
bool dijkstra(const unsigned int s, const unsigned int t, const unsigned int n)
{
    using pair_t = pair<int, unsigned int>;
    static bool vis[maxn + 1];
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    fill(dis, dis + 1 + n, inf);
    fill(vis, vis + 1 + n, false);
    dis[s] = 0;
    q.emplace(0, s);
    while (!q.empty())
    {
        const auto cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (Edge* i = head[cur.second]; i; i = i->pre)
        {
            const int w = h[i->from] - h[i->to] + i->cost;
            if (!vis[i->to] && i->cap && dis[i->to] > cur.first + w)
            {
                dis[i->to] = cur.first + w;
                q.emplace(dis[i->to], i->to);
            }
        }
    }
    return vis[t];
}
int dfs(const int x, const int cap, const int sink)
{
    static bool vis[maxn + 1];
    if (x == sink || !cap)
        return cap;
    vis[x] = true;
    int rst = cap;
    for (Edge* i = head[x]; i && rst; i = i->pre)
    {
        const int w = h[i->from] + i->cost - h[i->to];
        if (!vis[i->to] && dis[i->to] == dis[x] + w)
        {
            const int t = dfs(i->to, min(rst, i->cap), sink);
            rst -= t;
            i->cap -= t;
            ed[i->id ^ 1].cap += t;
        }
    }
    vis[x] = false;
    return cap - rst;
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
        unsigned int s, t;
        int c, w;
        cin >> s >> t >> c >> w;
        addEdge(s, t, c, w);
        addEdge(t, s, 0, -w);
    }
    int mxf = 0, mnc = 0;
    while (dijkstra(1, n, n))
    {
        const int t = dfs(1, inf, n);
        for (unsigned int i = 1; i <= n; ++i)
            if (dis[i] != inf)
                h[i] += dis[i];
        mxf += t;
        mnc += h[n] * t;
    }
    cout << mxf << " " << mnc << endl;
    return 0;
}