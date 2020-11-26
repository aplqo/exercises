#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxv = 100;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int p[maxn + 10], v;
unsigned long long g[maxn + 10], ans = 0;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
    g[from] += p[to];
    g[to] += p[from];
}
void dfs(const unsigned int x, const unsigned int f)
{
    static unsigned long long subFrom[maxv + 10], subTo[maxv + 10];
    static unsigned long long maxFrom[maxn + 10][maxv + 10], maxTo[maxn + 10][maxv + 10];
    ans = max(ans, g[x]);
    fill(maxTo[x] + 1, maxTo[x] + 1 + v, g[x]);
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        dfs(to, x);
        for (unsigned int j = v; j; --j)
        {
            subFrom[j] = max(maxFrom[to][j], maxFrom[to][j - 1] + g[to] - p[x]);
            subTo[j] = max(maxTo[to][j], maxTo[to][j - 1] + g[x] - p[to]);
            ans = max({ ans, maxFrom[x][v - j] + subTo[j], subFrom[j] + maxTo[x][v - j] });
        }
        for (unsigned int j = 1; j <= v; ++j)
        {
            maxFrom[x][j] = max({ maxFrom[x][j], maxFrom[x][j - 1], subFrom[j] });
            maxTo[x][j] = max({ maxTo[x][j], maxTo[x][j - 1], subTo[j] });
        }
    }
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> v;
    if (!v)
    {
        cout << "0\n";
        return 0;
    }
    copy_n(istream_iterator<unsigned int>(cin), n, p + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    dfs(1, 0);
    cout << ans << "\n";
    return 0;
}