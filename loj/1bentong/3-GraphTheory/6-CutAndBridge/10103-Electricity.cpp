#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 1e4;

vector<unsigned int> gra[maxn + 1];
unsigned int dfn[maxn + 1], low[maxn + 1], siz[maxn + 1], sta = 0;
int cnt[maxn + 1];

void tarjan(const unsigned int x, const unsigned int f, const unsigned int rt)
{
    dfn[x] = low[x] = ++sta;
    siz[x] = 1;
    unsigned int cn = 0;
    for (unsigned int to : gra[x])
    {
        if (to == f)
            continue;
        if (!dfn[to])
        {
            tarjan(to, x, rt);
            ++cn;
            siz[x] += siz[to];
            low[x] = min(low[x], low[to]);
            if (((x == rt && cn > 1) || (x != rt && dfn[x] <= low[to])))
                ++cnt[x];
        }
        else
            low[x] = min(low[x], dfn[to]);
    }
}
void solve()
{
    unsigned int n, c;
    cin >> n >> c;
    if (!n && !c)
        exit(0);
    for (unsigned int i = 0; i < c; ++i)
    {
        unsigned int u, v;
        cin >> u >> v;
        if (u == v)
            continue;
        gra[u].push_back(v);
        gra[v].push_back(u);
    }
    int blk = 0;
    for (unsigned int i = 0; i < n; ++i)
        if (!dfn[i])
        {
            tarjan(i, 0, i);
            ++blk;
            if (siz[i] == 1)
                --cnt[i];
        }
    cout << (*max_element(cnt, cnt + n) + blk) << "\n";
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    while (true)
    {
        solve();
        for_each(gra, gra + maxn, mem_fn(&vector<unsigned int>::clear));
        memset(cnt, 0, sizeof(cnt));
        memset(dfn, 0, sizeof(dfn));
        memset(low, 0, sizeof(low));
        sta = 0;
    }
    return 0;
}