#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1500;

unsigned int chl[maxn + 1][maxn + 1], cnt[maxn + 1];
unsigned int f[maxn + 1][2];

void dfs(const unsigned int x, const unsigned int fa)
{
    const unsigned int siz = cnt[x];
    for (unsigned int* i = chl[x]; i < chl[x] + siz; ++i)
    {
        const unsigned int to = *i;
        if (to == fa)
            continue;
        dfs(to, x);
        f[x][0] += f[to][1];
        f[x][1] += min(f[to][0], f[to][1]);
    }
    ++f[x][1];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int x;
        cin >> x;
        cin >> cnt[x];
        if (cnt[x])
            copy_n(istream_iterator<unsigned int>(cin), cnt[x], chl[x]);
    }
    dfs(0, 0);
    cout << min(f[0][0], f[0][1]) << endl;
    return 0;
}