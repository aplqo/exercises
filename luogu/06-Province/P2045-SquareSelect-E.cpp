#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 50;
const unsigned int maxv = maxn * maxn * 2 + 2, maxe = maxn * maxn * (2 + 2) * 2;
constexpr int inf = INT_MAX;

struct Edge
{
    unsigned int from, to;
    int cost;
    mutable int cap;
    const Edge *pre, *rev;
} ed[maxe + 1];
const Edge* head[maxv + 1];
int dis[maxv + 1];
int num[maxn + 1][maxn + 1];

void addDirEdge(const unsigned int from, const unsigned int to, const int cap, const int cost)
{
    static Edge* cur = ed;
    cur->from = from;
    cur->to = to;
    cur->cap = cap;
    cur->cost = cost;
    cur->rev = &ed[(cur - ed) ^ 1];
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
inline void addEdge(const unsigned int from, const unsigned int to, const int cap, const int cost)
{
    addDirEdge(from, to, cap, cost);
    addDirEdge(to, from, 0, -cost);
}
bool spfa(const unsigned int s, const unsigned int t, const unsigned int n)
{
    static bool vis[maxv + 1];
    fill(vis, vis + n, false);
    fill(dis, dis + n, -inf);
    queue<unsigned int> q;
    dis[s] = 0;
    q.push(s);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        vis[cur] = false;
        for (const Edge* i = head[cur]; i; i = i->pre)
            if (dis[i->to] < dis[cur] + i->cost && i->cap)
            {
                dis[i->to] = dis[cur] + i->cost;
                if (!vis[i->to])
                {
                    vis[i->to] = true;
                    q.push(i->to);
                }
            }
    }
    return dis[t] > 0;
}
int dfs(const unsigned int x, const int cap, const unsigned int sink)
{
    static bool vis[maxv + 1];
    if (!cap || x == sink)
        return cap;
    vis[x] = true;
    int rst = cap;
    for (const Edge* i = head[x]; i && rst; i = i->pre)
        if (!vis[i->to] && dis[i->to] == dis[x] + i->cost)
        {
            const int v = dfs(i->to, min(rst, i->cap), sink);
            i->cap -= v;
            i->rev->cap += v;
            rst -= v;
        }
    vis[x] = false;
    return cap - rst;
}
int flow(const unsigned int s, const unsigned int t, const unsigned int n)
{
    int mxc = 0;
    while (spfa(s, t, n))
    {
        const int v = dfs(s, inf, t);
        mxc += v * dis[t];
    }
    return mxc;
}
pair<unsigned int, unsigned int> buildGraph(const unsigned int n, const unsigned int k)
{
    enum
    {
        In = 0,
        Out = 1
    };
    const auto id = [n](const unsigned int x, const unsigned int y, const unsigned int dir) {
        return 2 * (x * n + y) + dir;
    };
    const unsigned int s = id(n - 1, n - 1, Out) + 1, t = s + 1;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < n; ++j)
        {
            addEdge(id(i, j, In), id(i, j, Out), 1, num[i][j]);
            addEdge(id(i, j, In), id(i, j, Out), inf, 0);
            if (i + 1 < n)
                addEdge(id(i, j, Out), id(i + 1, j, In), inf, 0);
            if (j + 1 < n)
                addEdge(id(i, j, Out), id(i, j + 1, In), inf, 0);
        }
    addEdge(s, id(0, 0, In), k, 0);
    addEdge(id(n - 1, n - 1, Out), t, k, 0);
    return make_pair(s, t);
}
int main()
{
#ifndef APTESt
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    cin >> n >> k;
    for (unsigned int i = 0; i < n; ++i)
        copy_n(istream_iterator<int>(cin), n, num[i]);
    const auto [s, t] = buildGraph(n, k);
    cout << flow(s, t, t + 1) << endl;
    return 0;
}