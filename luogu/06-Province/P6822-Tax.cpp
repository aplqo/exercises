#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5;
constexpr unsigned long long inf = ULLONG_MAX / 2;

struct BiEdge
{
    unsigned int to, val, id;
    const BiEdge* rev;
} oed[maxm * 2 + 10];
vector<BiEdge*> orig[maxn + 10];
struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxm * 10 + maxn * 2];
const Edge* head[maxm * 2 + 10];

inline void addBiEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static unsigned int sta = 0;
    static BiEdge* cur = oed;
    new (cur) BiEdge { to, val, ++sta, cur + 1 };
    orig[from].push_back(cur++);
    new (cur) BiEdge { from, val, ++sta, cur - 1 };
    orig[to].push_back(cur++);
}
inline void addDirEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur++;
}
static void buildGraph(const unsigned int n, const unsigned int m)
{
    for (auto i = orig + 1; i < orig + n + 1; ++i)
    {
        if (i->empty())
            continue;
        sort(i->begin(), i->end(), [](BiEdge* a, BiEdge* b) { return a->val < b->val; });
        for (auto it = i->cbegin() + 1; it != i->cend(); ++it)
        {
            const BiEdge *const cv = *it, *lst = *(it - 1);
            addDirEdge(cv->id, lst->id, 0);
            addDirEdge(lst->id, cv->id, cv->val - lst->val);
        }
        for (const auto e : *i)
            addDirEdge(e->id, e->rev->id, e->val);
    }
    for (auto it : orig[1])
        addDirEdge(0, it->id, it->val);
    for (auto it : orig[n])
        addDirEdge(it->rev->id, m * 2 + 1, it->val);
}
unsigned long long dijkstra(const unsigned int n)
{
    using Pair = pair<unsigned long long, unsigned int>;
    static bool vis[maxm * 2 + 10];
    static unsigned long long dis[maxm * 2 + 10];
    priority_queue<Pair, vector<Pair>, greater<Pair>> q;
    fill(dis, dis + 1 + n, inf);
    dis[0] = 0;
    q.emplace(0, 0);
    while (!q.empty())
    {
        const Pair cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (const Edge* i = head[cur.second]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (dis[to] > cur.first + i->val)
            {
                dis[to] = cur.first + i->val;
                q.emplace(dis[to], to);
            }
        }
    }
    return dis[n];
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
        unsigned int a, b, c;
        cin >> a >> b >> c;
        addBiEdge(a, b, c);
    }
    buildGraph(n, m);
    cout << dijkstra(m * 2 + 1) << "\n";
    return 0;
}
