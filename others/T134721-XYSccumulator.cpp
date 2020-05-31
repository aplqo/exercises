#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1000000;

unsigned int siz[maxn + 1], fa[maxn + 1];

inline void init(const unsigned int n)
{
    iota(fa, fa + n + 1, 0);
    fill(siz, siz + n + 1, 1);
}
unsigned int find(const unsigned int x)
{
    return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(const unsigned int x, const unsigned int y)
{
    unsigned int fx = find(x), fy = find(y);
    if (fx == fy)
        return;
    if (siz[fx] < siz[fy])
        swap(fx, fy);
    fa[fy] = fx;
    siz[fx] += siz[fy];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    init(n);
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int t;
        cin >> t;
        merge(i, t);
    }
    unsigned int ans = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ans += find(i) == i;
    cout << ans << endl;
    return 0;
}