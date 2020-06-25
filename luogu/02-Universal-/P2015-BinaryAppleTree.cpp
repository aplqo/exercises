#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 100;

struct edge
{
    unsigned int to;
    unsigned int val;
    edge* pre;
} ed[maxn * 2 + 10];
edge* head[maxn + 1];
unsigned int f[maxn + 1][maxn + 1];

void addEdge(const unsigned int from, const unsigned int to, const unsigned int val)
{
    static edge* cur = ed;
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
}
void dfs(const unsigned int x, const unsigned int fa, const unsigned int q)
{
    unsigned int l = 0, r = 0;
    for (edge* i = head[x]; i; i = i->pre)
    {
        const unsigned int to = i->to;
        if (to == fa)
            continue;
        dfs(to, x, q);
        for (unsigned int j = 1; j <= q; ++j)
            f[to][j] += i->val;
        if (l)
            r = to;
        else
            l = to;
    }
    if (!l && !r)
        return;
    for (unsigned int i = 2; i <= q; ++i)
        for (unsigned int j = 0; j < i; ++j)
            f[x][i] = max(f[x][i], f[l][j] + f[r][i - j - 1]);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, q;
    cin >> n >> q;
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int a, b, v;
        cin >> a >> b >> v;
        addEdge(a, b, v);
        addEdge(b, a, v);
    }
    dfs(1, 0, q + 1);
    cout << f[1][q + 1] << endl;
    return 0;
}