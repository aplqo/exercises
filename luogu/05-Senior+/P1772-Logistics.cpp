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
const unsigned int maxn = 100, maxm = 20;
constexpr unsigned int inf = UINT_MAX;

struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxm * maxm + 1];
const Edge* head[maxm + 1];
unsigned int notAvail[maxn + 1], add[maxn + 1], rem[maxn + 1];
unsigned int cost[maxn + 1][maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur++;
}
unsigned int dijkstra(const unsigned int m, const unsigned int disable)
{
    static bool vis[maxm + 1];
    static unsigned int dis[maxm + 1];
    using pair_t = pair<unsigned int, unsigned int>;
    priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
    fill(dis, dis + 1 + m, inf);
    fill(vis, vis + 1 + m, false);
    dis[1] = 0;
    q.emplace(0, 1);
    while (!q.empty())
    {
        const pair_t cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (const Edge* i = head[cur.second]; i; i = i->pre)
            if (!(disable & (1 << i->to)) && dis[i->to] > cur.first + i->val)
            {
                dis[i->to] = cur.first + i->val;
                q.emplace(dis[i->to], i->to);
            }
    }
    return dis[m];
}
static void init(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 1; i <= n; ++i)
        notAvail[i] = (notAvail[i - 1] | add[i]) ^ rem[i];
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int nav = 0;
        for (unsigned int j = i; j <= n; ++j)
        {
            nav |= notAvail[j];
            cost[i][j] = dijkstra(m, nav);
        }
    }
}
unsigned int dp(const unsigned int n, const unsigned int k)
{
    static unsigned int f[maxn + 1];
    f[1] = cost[1][1];
    for (unsigned int i = 2; i <= n; ++i)
    {
        f[i] = cost[1][i] == inf ? inf : cost[1][i] * i;
        for (unsigned int j = 1; j < i; ++j)
            if (cost[j + 1][i] != inf)
                f[i] = min(f[i], f[j] + cost[j + 1][i] * (i - j) + k);
    }
    return f[n];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, k, e;
    cin >> n >> m >> k >> e;
    for (unsigned int i = 0; i < e; ++i)
    {
        unsigned int a, b, v;
        cin >> a >> b >> v;
        addEdge(a, b, v);
        addEdge(b, a, v);
    }
    unsigned int d;
    cin >> d;
    for (unsigned int i = 0; i < d; ++i)
    {
        unsigned int p, a, b;
        cin >> p >> a >> b;
        add[a] |= 1 << p;
        rem[b + 1] |= 1 << p;
    }
    init(n, m);
    cout << dp(n, k) << endl;
    return 0;
}