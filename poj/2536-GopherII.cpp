#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream>
using namespace std;
const int maxn = 100, maxe = maxn * maxn, maxv = maxn * maxn;

struct edge
{
    unsigned int to;
    edge* pre;
} ed[maxe * 2 + 10], *cur = ed;
edge* head[maxv + 10];
struct pos
{
    double x, y;
} gop[maxn + 10], hole[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
namespace bipartite
{
    unsigned int mat[maxv + 10];
    bool vis[maxv + 10];

    bool dfs(const unsigned int x)
    {
        for (edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (vis[to])
                continue;
            vis[to] = true;
            if (!mat[to] || dfs(mat[to]))
            {
                mat[to] = x;
                return true;
            }
        }
        return false;
    }
    inline unsigned int Hungrian(const unsigned int n, const unsigned int m)
    {
        unsigned int ret = 0;
        for (unsigned int i = 1; i <= n; ++i)
        {
            fill(vis, vis + n + m + 1, false);
            if (dfs(i))
                ++ret;
        }
        return ret;
    }
}
inline void CreateGraph(const unsigned int n, const unsigned int m, const unsigned int v, const unsigned int s)
{
    const unsigned int d = v * s;
    for (unsigned int i = 0; i < n; ++i)
        for (unsigned int j = 0; j < m; ++j)
            if (hypot(gop[i].x - hole[j].x, gop[i].y - hole[j].y) < d)
            {
                AddEdge(i + 1, n + j + 1);
                AddEdge(n + 1 + j, i + 1);
            }
}
inline void Solve()
{
    unsigned int n, m, v, s;
    cin >> n >> m >> s >> v;
    if (cin.fail())
        exit(0);
    for (pos* i = gop; i < gop + n; ++i)
        cin >> i->x >> i->y;
    for (pos* j = hole; j < hole + m; ++j)
        cin >> j->x >> j->y;
    CreateGraph(n, m, v, s);
    cout << n - bipartite::Hungrian(n, m) << endl;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    while (true)
    {
        Solve();
        cur = ed;
        memset(head, 0, sizeof(head));
        memset(bipartite::mat, 0, sizeof(bipartite::mat));
    }
    return 0;
}