#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
struct Subtree
{
    unsigned int dist, w;
} sub[maxn + 10], *scur = sub;
struct Max2Val
{
    unsigned int fst, snd;
    inline void update(const unsigned int x);
};
unsigned int fa[maxn + 10], w[maxn + 10], dep[maxn + 10], pathLca[maxn + 10], degree[maxn + 10];
bool onPath[maxn + 10];
unsigned int dis[maxn + 10], m;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
inline void Max2Val::update(const unsigned int x)
{
    if (x > fst)
    {
        snd = fst;
        fst = x;
    }
    else
        snd = max(snd, x);
}
bool dfsFather(const unsigned int x, const unsigned int f)
{
    onPath[x] = x == m;
    fa[x] = f;
    dep[x] = dep[f] + 1;
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            ++degree[x];
            onPath[x] = dfsFather(i->to, x) || onPath[x];
        }
    pathLca[x] = onPath[x] ? dep[x] : 0;
    return onPath[x];
}
void dfsW(const unsigned int x, const unsigned int f, const unsigned int acc, const unsigned int pat)
{
    Max2Val mx { 0, 0 };
    if (!degree[x])
    {
        w[x] = acc + dep[x] - pat;
        return;
    }
    const unsigned int npt = max(pat, pathLca[x]), nacc = acc + degree[x] - 1, ndelt = (f && x != m && onPath[x]);
    bool onPathTo = false;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        if (onPath[to])
        {
            dfsW(to, x, nacc, npt);
            onPathTo = true;
        }
        else
        {
            dfsW(to, x, nacc - ndelt, npt);
            mx.update(w[to]);
        }
    }
    w[x] = degree[x] - onPathTo <= 1 ? acc + 1 + dep[x] - pat : mx.snd;
}
void dfsDistance(const unsigned int x, const unsigned int f)
{
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
        {
            dis[i->to] = dis[x] + 1;
            dfsDistance(i->to, x);
        }
}
static void initSub(const unsigned int r, const unsigned int m)
{
    dfsFather(r, 0);
    degree[r] = 1;
    dfsW(r, 0, 0, 1);
    dfsDistance(m, 0);
    {
        unsigned int lst = 0;
        for (unsigned int i = m; i != r; lst = i, i = fa[i])
        {
            for (const Edge* j = head[i]; j; j = j->pre)
                if (j->to != lst && j->to != fa[i])
                    *(scur++) = Subtree { dis[j->to], w[j->to] };
        }
    }
    sort(sub, scur, [](const Subtree& a, const Subtree& b) { return a.dist < b.dist; });
}
bool check(const unsigned int mid)
{
    const Subtree* ptr = sub;
    unsigned int b = 0, available = 0;
    for (unsigned int i = 0; i < dep[m] && ptr < scur; ++i)
    {
        ++available;
        for (const unsigned int oldb = b; ptr < scur && ptr->dist <= i + 1; ++ptr)
            if (ptr->w + oldb > mid)
            {
                ++b;
                if (!available)
                    return false;
                --available;
            }
        if (b > mid)
            return false;
    }
    return ptr == scur;
}
unsigned int binarySearch(const unsigned int n)
{
    int l = 0, r = 4 * n;
    while (l <= r)
    {
        const int m = (l + r) / 2;
        if (check(m))
            r = m - 1;
        else
            l = m + 1;
    }
    return l;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, r;
    cin >> n >> r >> m;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    initSub(r, m);
    cout << binarySearch(n) << "\n";
    return 0;
}