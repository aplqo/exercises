#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000, maxc = 5, maxn_treedp = 2000;
constexpr unsigned long long inf = ULLONG_MAX >> 1;

struct Edge
{
    unsigned int to;
    const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int c[maxn + 10], d[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
    static Edge* cur = ed;
    new (cur) Edge { to, head[from] };
    head[from] = cur++;
    new (cur) Edge { from, head[to] };
    head[to] = cur++;
}
namespace Greedy
{
    unsigned int seq[maxn + 10];
    int ic[maxn + 10], id[maxn + 10];

    unsigned long long minCost(const unsigned int n)
    {
        long long ret = 0;
        iota(seq, seq + n, 1);
        copy(c, c + n + 1, ic);
        copy(d, d + n + 1, id);
        partition(seq, seq + n, [](const unsigned int i) { return c[i]; });
        for (const unsigned int* i = seq; i < seq + n; ++i)
            if (ic[*i] == 1)
            {
                ret += id[*i];
                id[*i] = 0;
                for (const Edge* j = head[*i]; j; j = j->pre)
                    id[j->to] -= ic[*i];
            }
            else if (id[*i] > 0)
                ret += id[*i];
        return ret;
    }
}
namespace Dp
{
    unsigned long long cost[maxn_treedp + 10][2]; // 0: Get from subtree, 1: get from father
    unsigned long long sub[maxn_treedp + 10][maxn_treedp * maxc + 10]; // sub[i][j] get j from subtree of i

    void dfs(const unsigned int x, const unsigned int f)
    {
        unsigned int subc = 0;
        for (const Edge* i = head[x]; i; i = i->pre)
        {
            const unsigned int to = i->to;
            if (to == f)
                continue;
            dfs(i->to, x);
            ++subc;
            for (int i = subc * maxc; i >= 0; --i)
                if (i >= c[to])
                    sub[x][i] = min(sub[x][i] + cost[to][1], sub[x][i - c[to]] + cost[to][0]);
                else
                    sub[x][i] += cost[to][1];
        }
        for (unsigned int j = 0; j <= subc * maxc; ++j)
        {
            cost[x][0] = min(cost[x][0], sub[x][j] + d[x] - min(j, d[x]));
            cost[x][1] = min(cost[x][1], sub[x][j] + d[x] - min(j + c[f], d[x]));
        }
    }

    unsigned long long minCost(const unsigned int n)
    {
        for (unsigned int i = 1; i <= n; ++i)
        {
            cost[i][0] = cost[i][1] = inf;
            fill(sub[i] + 1, sub[i] + n * maxc + 1, inf);
        }
        dfs(1, 0);
        return cost[1][0];
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, d + 1);
    copy_n(istream_iterator<unsigned int>(cin), n, c + 1);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        addEdge(u, v);
    }
    if (all_of(c + 1, c + 1 + n, [](const unsigned int i) { return i < 2; }))
        cout << Greedy::minCost(n) << "\n";
    else
        cout << Dp::minCost(n) << "\n";
    return 0;
}