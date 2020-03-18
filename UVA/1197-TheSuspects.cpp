#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 30010;

int fa[maxn], siz[maxn];

int GetFather(int x)
{
    return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
void merge(int x, int y)
{
    int fx = GetFather(x), fy = GetFather(y);
    if (fx == fy)
        return;
    if (siz[fx] < siz[fy])
        swap(fx, fy);
    fa[fy] = fx;
    siz[fx] += siz[fy];
}

inline void solve()
{
    int n, m;
    cin >> n >> m;
    if (n == 0 && m == 0)
        exit(0);
    iota(fa, fa + n, 0);
    fill(siz, siz + n, 1);

    for (int i = 0; i < m; ++i)
    {
        int k, lst, cur;
        cin >> k;
        if (k == 0)
            continue;
        cin >> lst;
        for (int j = 1; j < k; ++j)
        {
            cin >> cur;
            merge(lst, cur);
        }
    }
    for (int i = 0; i < n; ++i)
        GetFather(i);
    cout << count(fa, fa + n, fa[0]);
}

int main()
{
    ios_base::sync_with_stdio(false);
    while (true)
    {
        solve();
        cout << endl;
    }
    return 0;
}