#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
using namespace std;
const unsigned int maxn = 1e4, maxk = 100, maxm = 1e4;
constexpr unsigned long long inf = ULLONG_MAX / 2;

struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxm + maxk + 10];
const Edge* head[maxn + 10];
struct Change
{
    unsigned int l, r;
    unsigned int from;
    Edge* ed;
} chg[maxk + 10];

inline Edge* addDirEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur;
    return cur++;
}
void dijkstra(const unsigned int n, const unsigned int s, unsigned long long res[])
{
    static unsigned int vis[maxn + 10], sta = 0;
    typedef pair<unsigned long long, unsigned int> Pair;
    priority_queue<Pair, vector<Pair>, greater<Pair>> q;
    fill(res, res + n + 1, inf);
    ++sta;
    res[s] = 0;
    q.emplace(0, s);
    while (!q.empty())
    {
        const Pair cur = q.top();
        q.pop();
        if (vis[cur.second] == sta)
            continue;
        vis[cur.second] = sta;
        for (const Edge* i = head[cur.second]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (res[to] > cur.first + i->val)
            {
                res[to] = cur.first + i->val;
                q.emplace(res[to], to);
            }
        }
    }
}
template <template <typename> class pred>
bool check(const unsigned int n, const unsigned int k, const unsigned int s1, const unsigned int s2, const unsigned int f)
{
    static pred<unsigned long long> cmp;
    static unsigned long long dist[2][maxn + 10];
    for (Change* i = chg; i < chg + k; ++i)
        i->ed->val = i->r;
    dijkstra(n, s1, dist[0]);
    dijkstra(n, s2, dist[1]);
    while (true)
    {
        bool upd = false;
        for (Change* i = chg; i < chg + k; ++i)
            if (i->ed->val != i->l && cmp(dist[0][i->from], dist[1][i->from]))
            {
                i->ed->val = i->l;
                upd = true;
                break;
            }
        if (!upd)
            break;
        dijkstra(n, s1, dist[0]);
        dijkstra(n, s2, dist[1]);
    }
    return cmp(dist[0][f], dist[1][f]);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, k, s1, s2, f;
    cin >> n >> m >> k >> s1 >> s2 >> f;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a, b, c;
        cin >> a >> b >> c;
        addDirEdge(a, b, c);
    }
    for (Change* i = chg; i < chg + k; ++i)
    {
        unsigned int to;
        cin >> i->from >> to >> i->l >> i->r;
        i->ed = addDirEdge(i->from, to, i->r);
    }
    if (check<less>(n, k, s1, s2, f))
        cout << "WIN\n";
    else if (check<less_equal>(n, k, s1, s2, f))
        cout << "DRAW\n";
    else
    {
        cout << "LOSE\n";
        return 0;
    }
    for (const Change* i = chg; i < chg + k; ++i)
        cout << i->ed->val << " ";
    cout.put('\n');
    return 0;
}