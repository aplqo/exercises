#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 2e5, maxm = 4e5, maxl = 22;
const unsigned int maxv = maxn + maxm, maxe = maxm * 2;
using dist_t = unsigned int;
const dist_t inf = numeric_limits<dist_t>::max() / 2;

struct Edge
{
    unsigned int from, to, hight;
    dist_t val;
    const Edge* pre;
} ed[maxm * 2], *cur = ed;
const Edge* head[maxn + 1];
struct VirtEdge
{
    unsigned int to;
    const VirtEdge* pre;
} ved[maxe + 1], *vcur = ved;
const VirtEdge* vhead[maxv + 1];
unsigned int vCnt;
dist_t dis[maxn + 1], mindis[maxv + 1];
unsigned int wight[maxv + 1], fa[maxv + 1][maxl + 1];
unsigned int lg[maxv + 1], dep[maxv + 1], n;

void addEdge(const unsigned int from, const unsigned int to, const dist_t val, const unsigned int h)
{
    cur->from = from;
    cur->to = to;
    cur->val = val;
    cur->hight = h;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void addVirtEdge(const unsigned int from, const unsigned int to)
{
    vcur->to = to;
    vcur->pre = vhead[from];
    vhead[from] = vcur;
    ++vcur;
}
inline void getLog(const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        lg[i] = lg[i >> 1] + 1;
}
namespace dsu
{
    unsigned int fa[maxn + 1], siz[maxn + 1], root[maxn + 1];

    inline void init(const unsigned int n)
    {
        fill(siz, siz + 1 + n, 1);
        iota(fa, fa + 1 + n, 0);
        copy(fa, fa + 1 + n, root);
    }
    unsigned int getFather(const unsigned int x)
    {
        return fa[x] == x ? x : fa[x] = getFather(fa[x]);
    }
    inline unsigned int mergeRoot(unsigned int rx, unsigned int ry)
    {
        if (siz[rx] < siz[ry])
            swap(rx, ry);
        fa[ry] = rx;
        siz[rx] += siz[ry];
        return rx;
    }
}
void dijkstra(const unsigned int n)
{
    static bool vis[maxn + 1];
    using Pair = pair<dist_t, unsigned int>;
    priority_queue<Pair, vector<Pair>, greater<Pair>> q;
    fill(dis, dis + 1 + n, inf);
    fill(vis, vis + 1 + n, false);
    q.emplace(0, 1);
    dis[1] = 0;
    while (!q.empty())
    {
        const Pair cur = q.top();
        q.pop();
        if (vis[cur.second])
            continue;
        vis[cur.second] = true;
        for (auto i = head[cur.second]; i; i = i->pre)
            if (dis[i->to] > cur.first + i->val)
            {
                dis[i->to] = cur.first + i->val;
                q.emplace(dis[i->to], i->to);
            }
    }
}
bool connect(const Edge* const i)
{
    const unsigned int fx = dsu::getFather(i->from), fy = dsu::getFather(i->to);
    if (fx == fy)
        return false;
    const unsigned int vp = ++vCnt;
    addVirtEdge(vp, dsu::root[fx]);
    addVirtEdge(vp, dsu::root[fy]);
    wight[vp] = i->hight;
    dsu::root[dsu::mergeRoot(fx, fy)] = vp;
    return true;
}
void kruskal(const unsigned int n, const unsigned int m)
{
    static Edge* tmp[maxm * 2];
    transform(ed, ed + m * 2, tmp, [](Edge& a) { return &a; });
    sort(tmp, tmp + m * 2, [](const Edge* a, const Edge* b) { return a->hight > b->hight; });
    unsigned int p = 0;
    for (unsigned int i = 1; i < n; ++i)
    {
        while (p < m * 2 && !connect(tmp[p]))
            ++p;
        ++p;
    }
}
void dfs(const unsigned int x, const unsigned int f)
{
    fa[x][0] = f;
    dep[x] = dep[f] + 1;
    mindis[x] = inf;
    for (unsigned int i = 1; i <= lg[dep[x]]; ++i)
        fa[x][i] = fa[fa[x][i - 1]][i - 1];
    if (x <= n)
    {
        mindis[x] = dis[x];
        return;
    }
    for (const VirtEdge* i = vhead[x]; i; i = i->pre)
    {
        dfs(i->to, x);
        mindis[x] = min(mindis[x], mindis[i->to]);
    }
}
static void init(const unsigned int n, const unsigned int m)
{
    dsu::init(n);
    vCnt = n;
    kruskal(n, m);
    dijkstra(n);
    dfs(vCnt, 0);
}
dist_t query(unsigned int v, const unsigned int h)
{
    for (unsigned int i = lg[dep[v]]; i;)
        if (wight[fa[v][i - 1]] > h)
            v = fa[v][i - 1];
        else
            --i;
    return mindis[v];
}

void solve()
{
    unsigned int m;
    cin >> n >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int u, v, a;
        dist_t l;
        cin >> u >> v >> l >> a;
        addEdge(u, v, l, a);
        addEdge(v, u, l, a);
    }
    init(n, m);
    unsigned int q, k, s;
    cin >> q >> k >> s;
    unsigned int lst = 0;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int v, p;
        cin >> v >> p;
        v = (v + lst * k - 1) % n + 1;
        p = (p + lst * k) % (s + 1);
        cout << (lst = query(v, p)) << endl;
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    getLog(maxn + maxm);
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        solve();
        vcur = ved;
        cur = ed;
        memset(head, 0, sizeof(head));
        memset(vhead, 0, sizeof(vhead));
        memset(fa, 0, sizeof(fa));
    }
    return 0;
}