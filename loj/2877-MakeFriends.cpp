#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <queue>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5;

struct Edge
{
    unsigned int from, to;
    const Edge* pre;
} ed[maxm + 10];
const Edge* head[maxn + 10];
unsigned int degree[maxn + 10];

namespace Disjoint
{
    unsigned int fa[maxn + 10], siz[maxn + 10];

    static void init(const unsigned int n)
    {
        iota(fa, fa + 1 + n, 0);
        fill(siz, siz + 1 + n, 1);
    }
    unsigned int find(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = find(fa[x]);
    }
    void merge(unsigned int x, unsigned int y)
    {
        x = find(x);
        y = find(y);
        if (x == y)
            return;
        if (siz[x] < siz[y])
            swap(x, y);
        fa[y] = x;
        siz[x] += siz[y];
    }
}

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { from, to, head[from] };
    head[from] = cur++;
    ++degree[from];
}
void bfs(const unsigned int n)
{
    static bool vis[maxn + 10];
    queue<unsigned int> q;
    const auto add = [&q](const unsigned int from, const unsigned int to) {
        Disjoint::merge(from, to);
        if (!vis[to])
        {
            vis[to] = true;
            q.push(to);
        }
    };
    for (unsigned int i = 1; i <= n; ++i)
        if (degree[i] >= 2)
        {
            const unsigned int rt = head[i]->to;
            for (const Edge* j = head[i]; j; j = j->pre)
                add(rt, j->to);
        }
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        for (const Edge* i = head[cur]; i; i = i->pre)
            add(cur, i->to);
    }
}
unsigned long long count(const unsigned int n, const unsigned int m)
{
    using namespace Disjoint;
    unsigned long long ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        if (find(i) == i)
            ret += siz[i] * static_cast<unsigned long long>(siz[i] - 1);
    for (const Edge* i = ed; i < ed + m; ++i)
        if (find(i->from) != find(i->to))
            ++ret;
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
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    Disjoint::init(n);
    bfs(n);
    cout << count(n, m) << "\n";
    return 0;
}