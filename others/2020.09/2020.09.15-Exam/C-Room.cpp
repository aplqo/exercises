#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 5000, maxm = 5500, maxk = 10;
constexpr unsigned int inf = UINT_MAX / 2;
constexpr unsigned int maxs = maxn * (1 << maxk), msk = (1 << maxk) - 1;

struct Edge
{
    unsigned int to, key;
    const Edge* pre;
} ed[maxm + 10];
const Edge* head[maxn + 10];
unsigned int dis[maxs + 10], key[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int key)
{
    static Edge* cur = ed;
    new (cur) Edge { to, key, head[from] };
    head[from] = cur++;
}
#define Key(s) ((s)&msk)
#define Vertex(s) ((s) >> maxk)
#define Id(v, k) (((v) << maxk) | (k))
void bfs()
{
    static bool vis[maxs + 10];
    fill(dis, dis + 1 + maxs, inf);
    queue<unsigned int> q;
    dis[Id(0, key[0])] = 0;
    q.emplace(Id(0, key[0]));
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        const unsigned int v = Vertex(cur), k = Key(cur);
        q.pop();
        for (const Edge* i = head[v]; i; i = i->pre)
        {
            if ((i->key & k) != i->key)
                continue;
            const unsigned int to = Id(i->to, k | key[i->to]);
            if (dis[to] > dis[cur] + 1)
            {
                dis[to] = dis[cur] + 1;
                if (!vis[to])
                {
                    q.push(to);
                    vis[to] = true;
                }
            }
        }
    }
}
unsigned int getMinDist(const unsigned int n, const unsigned int k)
{
    unsigned int ret = inf;
    for (unsigned int i = 0; i < (1 << k); ++i)
        ret = min(ret, dis[Id(n - 1, i)]);
    return ret;
}
unsigned int readKey(const unsigned int k)
{
    unsigned int ret = 0;
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int v;
        cin >> v;
        ret |= v << i;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, k;
    cin >> n >> m >> k;
    for (unsigned int i = 0; i < n; ++i)
        key[i] = readKey(k);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u - 1, v - 1, readKey(k));
    }
    bfs();
    const unsigned int v = getMinDist(n, k);
    if (v != inf)
        cout << v << "\n";
    else
        cout << "No Solution\n";
    return 0;
}