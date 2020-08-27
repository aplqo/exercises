#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 2e5;
const unsigned long long mod = 1e9 + 7;

struct Edge
{
    unsigned int to;
    const Edge *pre, *rev;
} ed[maxn * 2 + 1];
const Edge* head[maxn + 1];
unsigned int dep[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from], cur + 1 };
    head[from] = cur++;
    new (cur) Edge { from, head[to], cur - 1 };
    head[to] = cur++;
}
unsigned int dfs(const unsigned int x, const Edge* fe)
{
    unsigned int ret = 0;
    for (const Edge* i = head[x]; i; i = i->pre)
    {
        if (i == fe)
            continue;
        if (dep[i->to] && !ret)
            ret = dep[x] - dep[i->to];
        else if (!dep[i->to])
        {
            dep[i->to] = dep[x] + 1;
            const unsigned int t = dfs(i->to, i->rev);
            if (t)
                ret = t;
        }
    }
    return ret;
}
unsigned long long quickPow(unsigned int n)
{
    unsigned long long ret = 1;
    for (unsigned long long v = 2; n; n >>= 1)
    {
        if (n & 0x01)
            ret = (ret * v) % mod;
        v = (v * v) % mod;
    }
    return ret;
}
unsigned long long count(const unsigned int n)
{
    unsigned long long ret = 1;
    unsigned int e = 0;
    for (unsigned int i = 0; i < n; ++i)
    {
        if (dep[i])
            continue;
        dep[i] = 1;
        const unsigned int len = dfs(i, 0) + 1;
        if (len > 1)
        {
            ret = (ret * (quickPow(len) + mod - 2) % mod) % mod;
            e += len;
        }
    }
    ret = (ret * quickPow(n - e)) % mod;
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int t;
        cin >> t;
        addEdge(i, t - 1);
    }
    cout << count(n) << "\n";
    return 0;
}