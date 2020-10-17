#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 500, maxm = 500;

unsigned int cnt[maxm + 1][2];

void solve()
{
    static char str[maxm + 10];
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> str;
        for (unsigned int j = 0; j < m; ++j)
            ++cnt[j][str[j] - '0'];
    }
    unsigned int ret = 0;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int cv = 0;
        if (!cnt[i][0])
            ++cv;
        if (cnt[i][1] < 2)
            cv += 2 - cnt[i][1];
        ret = max(ret, cv);
    }
    cout << ret << "\n";
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int t;
    cin >> t;
    for (unsigned int i = 0; i < t; ++i)
    {
        solve();
        memset(cnt, 0, sizeof(cnt));
    }
    return 0;
}