#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
constexpr unsigned int inf = UINT_MAX >> 1;
const int maxn = 300;

unsigned int sum[maxn + 10], a[maxn + 10];
unsigned int f[maxn + 10][maxn + 10];

void init(const unsigned int n)
{
    partial_sum(a, a + n + 1, sum);
    for (auto& i : f)
        fill(i, i + n + 1, inf);
    for (unsigned int i = 0; i <= n; ++i)
        f[i][i] = 0;
}
unsigned int dp(const unsigned int n)
{
    for (unsigned int i = 1; i < n; ++i)
        for (unsigned int j = 1; j + i <= n; ++j)
            for (unsigned int k = j; k < j + i; ++k)
                f[j][j + i] = min(f[j][j + i], f[j][k] + f[k + 1][j + i] + sum[j + i] - sum[j - 1]);
    return f[1][n];
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (unsigned int* i = a + 1; i < a + 1 + n; ++i)
        cin >> *i;
    init(n);
    cout << dp(n) << endl;
    return 0;
}