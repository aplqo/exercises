#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxn = 2000, maxm = 10000;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm + 10];
const Edge* head[maxn + 10];
unsigned int k[maxn + 10];
unsigned int vis[maxn + 10], degree[maxn + 10], sta;
vector<unsigned int> before[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    ++degree[to];
}
void topological(const unsigned int n)
{
    queue<unsigned int> q;
    for (unsigned int i = 1; i <= n; ++i)
        if (!degree[i])
            q.push(i);
    while (!q.empty())
    {
        const unsigned int cur = q.front(), nv = k[cur] - 1;
        q.pop();
        for (const Edge* i = head[cur]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            k[to] = min(k[to], nv);
            --degree[to];
            if (!degree[to])
                q.push(to);
        }
    }
}
void markFrom(const unsigned int x)
{
    if (vis[x] == sta)
        return;
    vis[x] = sta;
    for (const Edge* i = head[x]; i; i = i->pre)
        markFrom(i->to);
}
static void initK(const unsigned int n)
{
    topological(n);
    for (auto i = before; i < before + n + 1; ++i)
        i->reserve(n);
    for (unsigned int i = 1; i <= n; ++i)
        before[k[i]].emplace_back(i);
}
unsigned int greedyEarly(const unsigned int p, const unsigned int n)
{
    ++sta;
    markFrom(p);
    unsigned int cur = n;
    for (unsigned int i = n; i > 0; --i)
    {
        if (cur > i)
            return cur;
        for (unsigned int x : before[i])
            if (vis[x] != sta)
                --cur;
    }
    return cur;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    copy_n(istream_iterator<unsigned int>(cin), n, k + 1);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(v, u);
    }
    initK(n);
    for (auto i = before + 1; i < before + 1 + n; ++i)
        copy(i->cbegin(), i->cend(), ostream_iterator<unsigned int>(cout, " "));
    cout.put('\n');
    for (unsigned int i = 1; i <= n; ++i)
        cout << greedyEarly(i, n) << " ";
    cout.put('\n');
    return 0;
}