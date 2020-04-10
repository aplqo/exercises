#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 3000;

int a[maxn + 10], b[maxn + 10];
unsigned int f[maxn + 10][maxn + 10];

unsigned int dp(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        unsigned int v = 0;
        for (unsigned int j = 1; j <= n; ++j)
        {
            if (a[i] == b[j])
                f[i][j] = v + 1;
            else
                f[i][j] = f[i - 1][j];
            if (b[j] < a[i])
                v = max(v, f[i - 1][j]);
        }
    }
    unsigned int ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
        ret = max(ret, *max_element(f[i], f[i] + n + 1));
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (int* i = a + 1; i < a + n + 1; ++i)
        cin >> *i;
    for (int* i = b + 1; i < b + n + 1; ++i)
        cin >> *i;
    cout << dp(n) << endl;
    return 0;
}