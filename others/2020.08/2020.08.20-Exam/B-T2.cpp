#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <list>
#include <numeric>
using namespace std;
const unsigned int maxm = 5e5, maxn = 1e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
struct Con
{
    unsigned int a, b, d;

    inline unsigned int minDep() const;
} c[maxm + 1];
unsigned int dep[maxn + 1];

inline unsigned int Con::minDep() const
{
    return max(int(dep[a] + dep[b]) - int(d), 0);
}
void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    cur->to = to;
    cur->pre = head[from];
    head[from] = cur++;
}
void dfsDep(const unsigned int x, const unsigned int f, unsigned int dep[])
{
    dep[x] = dep[f] + 1;
    for (const Edge* i = head[x]; i; i = i->pre)
        if (i->to != f)
            dfsDep(i->to, x, dep);
}
bool check(const unsigned int x, const unsigned int m)
{
    static unsigned int dis[maxn + 1];
    dfsDep(x, 0, dis);
    for (const Con* i = c; i < c + m; ++i)
        if (dis[i->a] + dis[i->b] - 2 > i->d)
            return false;
    return true;
}
unsigned int findVert(const unsigned int n, const Con* c)
{
    static unsigned int disa[maxn + 1], disb[maxn + 1];
    dfsDep(c->a, 0, disa);
    dfsDep(c->b, 0, disb);
    unsigned int ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        if (disa[i] + disb[i] - 2 <= c->d && (!ret || dep[i] < dep[ret]))
            ret = i;
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
        addEdge(v, u);
    }
    dfsDep(1, 0, dep);
    unsigned int w = 0;
    for (Con* i = c; i < c + m; ++i)
    {
        cin >> i->a >> i->b >> i->d;
        w = max(w, i->minDep());
    }
    const unsigned int p = findVert(n, find_if(c, c + m, [w](const Con& i) { return i.minDep() == w; }));
    if (!p || !check(p, m))
    {
        cout << "NO" << '\n';
        return 0;
    }
    cout << p << '\n';
    return 0;
}