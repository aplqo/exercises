#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1000;

struct edge
{
    unsigned int to;
    edge* pre = nullptr;
} ed[maxn * maxn + 10];
edge* head[maxn + 10];

bool mat[maxn + 10][maxn + 10];
bool col[maxn + 10], vis[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
inline void CreateGraph(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = 1; j <= n; ++j)
            if (!mat[i][j] && i != j)
                AddEdge(i, j);
}
bool dfs(const unsigned int x)
{
    vis[x] = true;
    for (edge* i = head[x]; i; i = i->pre)
    {
        if (vis[i->to] && col[i->to] == col[x])
            return false;
        else if (vis[i->to])
            continue;
        col[i->to] = !col[x];
        if (!dfs(i->to))
            return false;
    }
    return true;
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
        unsigned int u, v;
        cin >> u >> v;
        mat[u][v] = true;
        mat[v][u] = true;
    }
    CreateGraph(n);
    for (unsigned int i = 1; i <= n; ++i)
        if (!vis[i] && !dfs(i))
        {
            cout << "-1" << endl;
            return 0;
        }
    const unsigned int c1 = count(col + 1, col + 1 + n, true);
    const unsigned int c2 = count(col + 1, col + 1 + n, false);
    cout << min(c1, c2) << endl;
    return 0;
}