#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000, maxm = 6000000;

struct Edge
{
    unsigned int to;
    const Edge *pre, *rev;
} ed[maxn * 4 + 10];
const Edge* head[maxn + 10];
unsigned int dfn[maxn + 10], low[maxn + 10];
class Disjoint
{
public:
    void init(const unsigned int n);
    inline bool isConnect(const unsigned int x, const unsigned int y);
    void merge(unsigned int x, unsigned int y);

private:
    unsigned int find(const unsigned int x);

    unsigned int fa[maxn + 10], siz[maxn + 10];
} con, bcc;

void Disjoint::init(const unsigned int n)
{
    iota(fa, fa + 1 + n, 0);
    fill(siz, siz + 1 + n, 1);
}
inline bool Disjoint::isConnect(const unsigned int x, const unsigned int y)
{
    return find(x) == find(y);
}
void Disjoint::merge(unsigned int x, unsigned int y)
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
unsigned int Disjoint::find(const unsigned int x)
{
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from], cur + 1 };
    head[from] = cur++;
    new (cur) Edge { from, head[to], cur - 1 };
    head[to] = cur++;
}
void tarjan(const unsigned int x, const Edge* const fe)
{
    static unsigned int sta = 0;
    dfn[x] = low[x] = ++sta;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (i == fe)
            continue;
        if (!dfn[to])
        {
            tarjan(to, i->rev);
            low[x] = min(low[x], low[to]);
            if (dfn[x] < low[to])
                cout << i->to << " " << i->rev->to << "\n";
        }
        else
            low[x] = min(low[x], dfn[to]);
    }
}

int main()
{
    // #ifndef APTEST
    ios_base::sync_with_stdio(false);
    // #endif
    unsigned int n, m;
    cin >> n >> m;
    con.init(n);
    bcc.init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int x, y;
        cin >> x >> y;
        if (!con.isConnect(x, y))
            con.merge(x, y);
        else if (!bcc.isConnect(x, y))
            bcc.merge(x, y);
        else
            continue;
        addEdge(x, y);
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (!dfn[i])
            tarjan(i, nullptr);
    return 0;
}