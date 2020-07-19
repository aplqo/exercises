#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 30000;

unsigned int a[maxn + 1];

unsigned int lis(const unsigned int n)
{
    static unsigned int f[maxn + 1];
    unsigned int mv[4] = {};
    memset(f, 0, sizeof(f));
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 1; j <= a[i]; ++j)
            f[i] = max(mv[j] + 1, f[i]);
        mv[a[i]] = max(mv[a[i]], f[i]);
    }
    return n - *max_element(f, f + n);
}

int main()
{
#ifndef APEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int* i = a; i < a + n; ++i)
        cin >> *i;
    unsigned int ans = lis(n);
    reverse(a, a + n);
    cout << min(ans, lis(n)) << endl;
    return 0;
}
