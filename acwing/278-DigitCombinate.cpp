#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100, maxm = 10000;

unsigned int a[maxn + 10], f[maxm + 10] = { 1 };

unsigned int dp(const unsigned int n, const unsigned int m)
{
    for (unsigned int i = 1; i <= n; ++i)
        for (unsigned int j = m; j >= a[i]; --j)
            f[j] += f[j - a[i]];
    return f[m];
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int* i = a + 1; i < a + 1 + n; ++i)
        cin >> *i;
    cout << dp(n, m) << endl;
    return 0;
}