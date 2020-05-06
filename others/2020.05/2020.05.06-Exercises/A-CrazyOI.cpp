/*Luogu team T132648: Crazy OI*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
using namespace std;
const int maxm = 500000, maxn = 12;

unsigned int c[maxn + 10], p[maxn + 10], l[maxn + 10];
unsigned int vis[maxm + 10], pos[maxn + 10];
unsigned int n, ml;

bool test(const unsigned int m)
{
    static unsigned int sta = 0;
    copy(c, c + n, pos);
    for (unsigned int i = 0; i <= ml; ++i, ++sta)
    {
        ++sta;
        for (unsigned int j = 0; j < n; ++j)
        {
            if (i > l[j])
                continue;
            if (vis[pos[j]] == sta)
                return false;
            vis[pos[j]] = sta;
            pos[j] = (pos[j] + p[j]) % m;
        }
    }
    return true;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int lv = 0, rv = maxm;
    cin >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> c[i] >> p[i] >> l[i];
        --c[i];
    }
    lv = *max_element(c, c + n);
    ml = *max_element(l, l + n);
    ++lv;
    for (; lv <= maxm; ++lv)
        if (test(lv))
            break;
    cout << lv << endl;
    return 0;
}
