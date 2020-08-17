#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 100, maxk = 50;
constexpr unsigned int inf = UINT_MAX / 2;

struct Edge
{
    unsigned int to, val;
};
vector<Edge> gra[maxn + 1];
unsigned int dis[maxn + 1], w[maxn + 1];
unsigned int f[maxn + 1][maxn + 1][maxk + 1], g[maxn + 1][maxk + 1];
unsigned int k;

void kp(const unsigned int x, const unsigned int fa, const unsigned int k)
{
    if (!gra[x].size())
    {
        memset(g, 0, sizeof(g));
        return;
    }
    for (unsigned int i = 1; i < gra[x].size(); ++i)
        fill(g[i], g[i] + k + 1, inf);
    copy(f[gra[x][0].to][fa], f[gra[x][0].to][fa] + k + 1, g[0]);
    for (unsigned int i = 1; i < gra[x].size(); ++i)
        for (unsigned int j = 0; j <= k; ++j)
            for (unsigned int t = 0; t <= j; ++t)
                g[i][j] = min(g[i][j], g[i - 1][t] + f[gra[x][i].to][fa][j - t]);
}
void dfs(const unsigned int x)
{
    static unsigned int stk[maxn + 1], *top = stk;
    *(top++) = x;
    for (Edge i : gra[x])
    {
        dis[i.to] = dis[x] + i.val;
        dfs(i.to);
    }
    const size_t cs = gra[x].size();

    if (x == 0)
    {
        kp(x, x, k);
        copy(g[cs - 1], g[cs - 1] + k + 1, f[0][0]);
        return;
    }

    // not select x
    for (unsigned int* i = stk; i < top - 1; ++i)
    {
        kp(x, *i, k);
        copy(g[cs - 1], g[cs - 1] + k + 1, f[x][*i]);
    }
    for (unsigned int* i = stk; i < top - 1; ++i)
        for (unsigned int j = 0; j <= k; ++j)
            f[x][*i][j] += (dis[x] - dis[*i]) * w[x];

    // select x
    for (unsigned int* i = stk; i < top - 1; ++i)
    {
        kp(x, x, k);
        for (unsigned int j = 1; j <= k; ++j)
            f[x][*i][j] = min(f[x][*i][j], g[cs - 1][j - 1]);
    }
    --top;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n >> k;
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int d, v;
        cin >> w[i] >> v >> d;
        gra[v].push_back(Edge { i, d });
    }
    dfs(0);
    cout << f[0][0][k] << endl;
    return 0;
}