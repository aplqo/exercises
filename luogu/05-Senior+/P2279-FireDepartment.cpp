#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
#include <utility>
using namespace std;
const unsigned int maxn = 1000;

struct edge
{
    unsigned int to;
    edge* pre;
} ed[maxn * 4 + 10];
edge* head[maxn + 1];
unsigned int fa[maxn + 1], degree[maxn + 1];
bool watch[maxn + 1], sel[maxn + 1], vis[maxn + 1];
pair<unsigned int, unsigned int> tmp[maxn + 1], *cur = tmp;

void addEdge(const unsigned int from, const unsigned int to)
{
    static edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur;
    ++degree[from];
    ++cur;
}
void dfsDep(const unsigned int x, const unsigned int dep)
{
    if (!degree[x])
    {
        *(cur++) = make_pair(dep, x);
        return;
    }
    for (edge* i = head[x]; i; i = i->pre)
        dfsDep(i->to, dep + 1);
}
void dfs(const unsigned int x, const unsigned int dep = 2)
{
    watch[x] = true;
    if (!dep)
        return;
    if (fa[x])
        dfs(fa[x], dep - 1);
    for (edge* i = head[x]; i; i = i->pre)
        dfs(i->to, dep - 1);
}
inline void setWatch(const unsigned int x)
{
    sel[x] = true;
    dfs(x);
}
void bfs(const unsigned int n)
{
    queue<unsigned int> q;
    sort(tmp, cur);
    for (const auto* i = cur - 1; i >= tmp; --i)
        q.push(i->second);
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        vis[cur] = true;
        if (!watch[cur])
        {
            if (fa[fa[cur]])
                setWatch(fa[fa[cur]]);
            else if (fa[cur])
                setWatch(fa[cur]);
            else
                setWatch(cur);
        }
        if (!vis[fa[cur]])
            q.push(fa[cur]);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 2; i <= n; ++i)
    {
        cin >> fa[i];
        addEdge(fa[i], i);
    }
    dfsDep(1, 0);
    bfs(n);
    cout << count(sel + 1, sel + 1 + n, true) << endl;
    return 0;
}