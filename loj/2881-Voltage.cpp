#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5;

enum part
{
    Even = 0,
    Odd = 1
};
struct Edge
{
    unsigned int to;
    const Edge *pre, *rev;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
unsigned int dep[maxn + 10];
unsigned int cycle[2];
int cov[2][maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from], cur + 1 };
    head[from] = cur++;
    new (cur) Edge { from, head[to], cur - 1 };
    head[to] = cur++;
}
void dfsCycle(const unsigned int x, const Edge* const fe)
{
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (i == fe)
            continue;
        if (!dep[to])
        {
            dep[to] = dep[x] + 1;
            dfsCycle(to, i->rev);
            cov[Even][x] += cov[Even][to];
            cov[Odd][x] += cov[Odd][to];
        }
        else if (dep[to] < dep[x])
        {
            const unsigned typ = (dep[x] - dep[to] + 1) & 0x01;
            ++cov[typ][x];
            --cov[typ][to];
            ++cycle[typ];
        }
    }
}
unsigned int countEdge(const unsigned int n)
{
    unsigned int ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        for (const Edge* j = head[i]; j; j = j->pre)
            if (dep[j->to] + 1 == dep[i])
            {
                if (!cov[Even][i] && cov[Odd][i] == cycle[Odd])
                    ++ret;
            }
    if (cycle[Odd] == 1)
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
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (!dep[i])
        {
            dep[i] = 1;
            dfsCycle(i, nullptr);
        }
    cout << countEdge(n) << "\n";
    return 0;
}