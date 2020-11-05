#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <iterator>
#include<algorithm>
using namespace std;
const unsigned int maxn = 2e5, maxm = 5e5;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
bool typ[maxn + 10], vis[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
unsigned int dfs(const unsigned int x)
{
    unsigned int ret = 1;
    vis[x] = true;
    for (const Edge* i = head[x]; i; i = i->pre)
        if (!vis[i->to])
        {
            typ[i->to] = !typ[x];
            ret += dfs(i->to);
        }
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
        unsigned int a, b;
        cin >> a >> b;
        addEdge(a, b);
    }
    for (unsigned int i = 1; i <= n; ++i)
        if (!vis[i] && dfs(i) == 1)
        {
            cout << "NIE\n";
            return 0;
        }
    cout << "TAK\n";
    transform(typ + 1, typ + 1 + n, ostream_iterator<char>(cout, "\n"), [](bool i) { return i ? 'K' : 'S'; });
    return 0;
}