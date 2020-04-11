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
const int maxn = 1e5, maxl = 32;

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
        copy(a, a + n, tab[0]);
        for (unsigned int i = 1; i <= lg[n]; ++i)
            for (unsigned int j = 0; j + (1 << i) <= n; ++j)
                tab[j][i] = min(tab[j][i - 1], tab[j + (1 << (i - 1))][i - 1]);
    }
    num_t query(const unsigned int l, const unsigned int r)
    {
        const unsigned int t = lg[r - l + 1];
        return min(tab[l][t], tab[r - (1 << t) + 1][t]);
    }
}

num_t GetMod(const unsigned int l, const unsigned int r, num_t x)
{
    const num_t m = st::query(l, r);
    for (unsigned int i = l; i <= r && x > m; ++i)
        x %= a[i];
    return x;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (num_t* i = a; i < a + n; ++i)
        cin >> *i;
    st::GetLog(n);
    st::init(n);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int l, r;
        num_t x;
        cin >> x >> l >> r;
        cout << GetMod(l - 1, r - 1, x) << endl;
    }
    return 0;
}