/*
* Luogu team T129186: Mod
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num_t = unsigned int;
const int maxn = 1e5, maxl = 27;

num_t a[maxn + 10];
namespace st
{
    unsigned int lg[maxn + 10];
    num_t tab[maxn + 10][maxl + 10];

    void GetLog(const unsigned int n)
    {
        for (unsigned int i = 2; i <= n; ++i)
            lg[i] = lg[i >> 1] + 1;
    }
    void init(const unsigned int n)
    {
        for (unsigned int i = 1; i <= n; ++i)
            tab[i][0] = a[i];
        for (unsigned int i = 1; i <= lg[n]; ++i)
            for (unsigned int j = 1; j + (1 << i) <= n; ++j)
                tab[j][i] = min(tab[j][i - 1], tab[j + (1 << (i - 1))][i - 1]);
    }
    num_t query(const unsigned int l, const unsigned int r)
    {
        const unsigned int t = lg[r - l + 1];
        return min(tab[l][t], tab[r - (1 << t) + 1][t]);
    }
}

unsigned int query(int l, int r, const num_t x)
{
    while (l <= r)
    {
        const int m = (l + r) >> 1;
        if (st::query(l, m) <= x)
            r = m - 1;
        else
            l = m + 1;
    }
    return l;
}

num_t GetMod(const unsigned int l, const unsigned int r, num_t x)
{
    for (unsigned int i = l; i <= r; ++i)
    {
        i = query(i, r, x);
        if (i <= r)
            x %= a[i];
    }
    return x;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (num_t* i = a + 1; i < a + n + 1; ++i)
        cin >> *i;
    st::GetLog(n);
    st::init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        num_t x;
        cin >> x >> l >> r;
        cout << GetMod(l, r, x) << endl;
    }
    return 0;
}