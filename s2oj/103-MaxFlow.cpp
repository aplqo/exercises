#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
const unsigned int maxn = 100, maxm = 5000;
typedef long long Flow;
constexpr Flow inf = numeric_limits<Flow>::max();

struct Edge
{
    unsigned int from, to;
    Flow cap;
    mutable Flow flw;
    const Edge *pre, *rev;
} ed[maxm * 2 + 10];
const Edge *head[maxn + 10], *par[maxn + 10];
unsigned int dep[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to, const Flow cap)
{
    static Edge* cur = ed;
    new (cur) Edge { from, to, cap, 0, head[from], cur + 1 };
    head[from] = cur++;
    new (cur) Edge { to, from, 0, 0, head[to], cur - 1 };
    head[to] = cur++;
}
void bfs(const unsigned int t)
{
    static bool vis[maxn + 10];
    queue<unsigned int> q;
    q.push(t);
    vis[t] = true;
    while (!q.empty())
    {
        const unsigned int cur = q.front();
        q.pop();
        for (const Edge* i = head[cur]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (!vis[to] && i->rev->cap > i->rev->flw)
            {
                vis[to] = true;
                dep[to] = dep[cur] + 1;
                q.push(to);
            }
        }
    }
}
Flow augment(const unsigned int s, const unsigned int t)
{
    Flow ret = inf;
    for (unsigned int x = t; x != s; x = par[x]->from)
        ret = min(ret, par[x]->cap - par[x]->flw);
    for (unsigned int x = t; x != s; x = par[x]->from)
    {
        par[x]->flw += ret;
        par[x]->rev->flw -= ret;
    }
    return ret;
}
Flow isap(const unsigned int n, const unsigned int s, const unsigned int t)
{
    static unsigned int cnt[maxn + 10];
    static const Edge* cur[maxn + 10];
    bfs(t);
    for (unsigned int i = 0; i <= n; ++i)
        ++cnt[dep[i]];
    copy(head, head + n + 1, cur);
    unsigned int x = s;
    Flow ret = 0;
    while (dep[s] < n)
    {
        if (x == t)
        {
            ret += augment(s, t);
            x = s;
        }
        bool adv = false;
        for (const Edge*& i = cur[x]; i; i = i->pre)
            if (dep[i->to] + 1 == dep[x] && i->cap > i->flw)
            {
                adv = true;
                par[i->to] = i;
                x = i->to;
                break;
            }
        if (!adv)
        {
            unsigned int miv = n;
            for (const Edge* i = head[x]; i; i = i->pre)
                if (i->cap > i->flw)
                    miv = min(miv, dep[i->to]);
            --cnt[dep[x]];
            if (!cnt[dep[x]])
                break;
            dep[x] = miv + 1;
            ++cnt[dep[x]];
            cur[x] = head[x];
            if (x != s)
                x = par[x]->from;
        }
    }
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, s, t;
    cin >> n >> m >> s >> t;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v, w;
        cin >> u >> v >> w;
        addEdge(u, v, w);
    }
    cout << isap(n, s, t) << "\n";
    return 0;
}