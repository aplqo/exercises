#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 3e5, maxm = 5e5;
constexpr unsigned long long mod = 998244353;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxm * 2 + 10];
const Edge* head[maxn + 10];
unsigned int dep[maxn + 10];
unsigned int cnt = 0;
unsigned long long ans = 1;

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
unsigned long long quickPow(unsigned long long a, unsigned int e)
{
    unsigned long long ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = (ret * a) % mod;
        a = (a * a) % mod;
    }
    return ret;
}

void dfs(const unsigned int x, const unsigned int f)
{
    dep[x] = dep[f] + 1;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == f)
            continue;
        else if (!dep[to])
            dfs(to, x);
        else if (dep[x] > dep[to])
        {
            const unsigned int ced = dep[x] - dep[to] + 1;
            cnt += ced;
            const unsigned long long cv = (quickPow(2, ced) + mod - 1) % mod;
            ans = (ans * cv) % mod;
        }
    }
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
            dfs(i, 0);
    ans = (ans * quickPow(2, m - cnt)) % mod;
    cout << ans << "\n";
    return 0;
}