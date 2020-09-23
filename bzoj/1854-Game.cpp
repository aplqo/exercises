#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000000, maxp = 10000;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[(maxn * 2 + maxp) * 2 + 10];
const Edge* head[maxn + maxp + 10];
unsigned int mat[maxn + maxp + 10];
bool vis[maxn + maxp + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
bool dfs(const unsigned int x)
{
    for (const Edge* i = head[x]; i; i = i->pre)
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
unsigned int solve()
{
    unsigned int ret = 0;
    for (unsigned int i = 1; i <= maxp; ++i)
    {
        memset(vis, 0, sizeof(vis));
        if (dfs(i))
            ++ret;
        else
            break;
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(i + maxp + 1, a);
        addEdge(i + maxp + 1, b);
    }
    cout << solve() << "\n";
    return 0;
}