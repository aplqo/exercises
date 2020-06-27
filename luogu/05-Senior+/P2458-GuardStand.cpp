#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1500;
constexpr unsigned int inf = UINT_MAX;

enum
{
    Father = 0, // guard by father
    Child = 1, // guard by child
    Self = 2 // guard by self
};
unsigned int chl[maxn + 1][maxn + 1], cnt[maxn + 1], k[maxn + 1];
unsigned int f[maxn + 1][3];
bool notRoot[maxn + 1];

void dfs(const unsigned int x, const unsigned int fa)
{
    const unsigned int *child = chl[x], siz = cnt[x];
    unsigned int minv = inf;
    for (const unsigned int* i = child; i < child + siz; ++i)
    {
        const unsigned int to = *i;
        if (to == fa)
            continue;
        dfs(to, x);
        const unsigned int cv = min(f[to][Child], f[to][Self]);
        f[x][Father] += cv;
        minv = min(minv, f[to][Self] - cv);
        f[x][Self] += min(f[to][Father], cv);
    }
    f[x][Self] += k[x];
    f[x][Child] = f[x][Father] + minv;
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
        cin >> k[x] >> cnt[x];
        for (unsigned int i = 0; i < cnt[x]; ++i)
        {
            cin >> chl[x][i];
            notRoot[chl[x][i]] = true;
        }
    }
    const unsigned int root = find(notRoot + 1, notRoot + 1 + n, false) - notRoot;
    dfs(root, 0);
    cout << min(f[root][Child], f[root][Self]) << endl;
    return 0;
}