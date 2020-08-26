#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000;

unsigned long long h[maxn + 1];
unsigned int lc[maxn + 1], rc[maxn + 1], siz[maxn + 1];

unsigned int build(const unsigned int n)
{
    static unsigned int stk[maxn + 10];
    unsigned int* top = stk;
    *(top++) = 1;
    unsigned int root = 1;
    for (unsigned int i = 2; i <= n; ++i)
    {
        unsigned int* ptr = top;
        while (ptr > stk && h[*(ptr - 1)] > h[i])
            --ptr;
        if (ptr > stk)
            rc[*(ptr - 1)] = i;
        else
            root = i;
        if (ptr < top)
            lc[i] = *ptr;
        top = ptr;
        *(top++) = i;
    }
    return root;
}
void dfs(const unsigned int x)
{
    if (!x)
        return;
    dfs(lc[x]);
    dfs(rc[x]);
    siz[x] = siz[lc[x]] + siz[rc[x]] + 1;
}
void solve()
{
    unsigned int n;
    cin >> n;
    if (!n)
        exit(0);
    copy_n(istream_iterator<unsigned int>(cin), n, h + 1);
    dfs(build(n));
    unsigned long long ans = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ans = max(ans, siz[i] * h[i]);
    cout << ans << "\n";
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    while (true)
    {
        solve();
        memset(lc, 0, sizeof(lc));
        memset(rc, 0, sizeof(rc));
    }
    return 0;
}