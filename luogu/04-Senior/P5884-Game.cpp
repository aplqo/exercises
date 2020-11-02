#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1500, maxr = maxn * (maxn - 1) / 2;

struct Edge
{
    unsigned int u, v;
    bool exist;
} ed[maxr + 10];
unsigned int fa[maxn + 10], siz[maxn + 10];

static void init(const unsigned int n)
{
    iota(fa, fa + 1 + n, 0);
    fill(siz, siz + 1 + n, 1);
}
unsigned int find(const unsigned int x)
{
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}
static bool tryMerge(unsigned int x, unsigned int y)
{
    x = find(x);
    y = find(y);
    if (x == y)
        return false;
    if (siz[x] < siz[y])
        swap(x, y);
    fa[y] = x;
    siz[x] += siz[y];
    return true;
}

void solve(const unsigned int n, const unsigned int r)
{
    init(n);
    for (Edge* i = ed + r - 1; i >= ed; --i)
        i->exist = tryMerge(i->u, i->v);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    const unsigned int r = n * (n - 1) / 2;
    for (Edge* i = ed; i < ed + r; ++i)
        cin >> i->u >> i->v;
    solve(n, r);
    for (const Edge* i = ed; i < ed + r; ++i)
        cout << i->exist << "\n";
    return 0;
}