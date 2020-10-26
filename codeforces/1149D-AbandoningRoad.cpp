#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
#include <queue>
using namespace std;
const unsigned int maxn = 70, maxm = 200, maxb = 70 / 4 + 1, maxs = 1 << maxb;
constexpr unsigned int inf = UINT_MAX / 2;

struct Edge
{
    unsigned int to, val;
    const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
unsigned int ans[maxn + 10], bid[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static Edge* cur = ed;
    new (cur) Edge { to, val, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, val, head[to] };
    head[to] = cur++;
}
namespace Disjoint
{
    unsigned int fa[maxn + 10], siz[maxn + 10];
    static void init(const unsigned int n)
    {
        fill(siz, siz + 1 + n, 1);
        iota(fa, fa + 1 + n, 0);
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
struct State
{
    unsigned int pos, dist, blk;
    inline bool operator<(const State& r) const { return dist > r.dist; }
    template <class T>
    inline T& operator[](T dat[maxn + 1][maxs + 1]) const { return dat[pos][blk]; }
};
static void initBlock(const unsigned int n)
{
    unsigned int sta = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        const unsigned int f = Disjoint::find(i);
        if (Disjoint::siz[f] > 3)
        {
            if (!bid[f])
                bid[f] = 1u << (++sta);
            bid[i] = bid[f];
        }
    }
}
void dijkstra(const unsigned int n, const unsigned int a)
{
    static bool vis[maxn + 1][maxs + 1];
    static unsigned int dis[maxn + 1][maxs + 1];
    priority_queue<State> q;
    for (unsigned int i = 0; i <= n; ++i)
        fill(dis[i], dis[i] + maxs + 1, inf);
    fill(ans, ans + 1 + n, inf);
    dis[1][bid[1]] = 0;
    q.push(State { 1, 0, bid[1] });
    while (!q.empty())
    {
        const State cur = q.top();
        q.pop();
        if (cur[vis])
            continue;
        ans[cur.pos] = min(ans[cur.pos], cur.dist);
        cur[vis] = true;
        for (const Edge* i = head[cur.pos]; i; i = i->pre)
        {
            using Disjoint::find;
            const unsigned int to = i->to;
            if (i->val != a && (find(to) == find(cur.pos) || (cur.blk & bid[to])))
                continue;
            const State sto { to, cur.dist + i->val, cur.blk | bid[to] };
            if (sto[dis] > sto.dist)
            {
                sto[dis] = sto.dist;
                q.push(sto);
            }
        }
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m, a, b;
    cin >> n >> m >> a >> b;
    Disjoint::init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v, c;
        cin >> u >> v >> c;
        addEdge(u, v, c);
        if (c == a)
            Disjoint::merge(u, v);
    }
    initBlock(n);
    dijkstra(n, a);
    copy(ans + 1, ans + 1 + n, ostream_iterator<unsigned int>(cout, " "));
    cout.put('\n');
    return 0;
}