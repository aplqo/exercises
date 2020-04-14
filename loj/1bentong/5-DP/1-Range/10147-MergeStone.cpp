#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
constexpr unsigned int inf = UINT_MAX >> 1;
const int maxn = 100 * 2;

unsigned int sum[maxn + 10], a[maxn + 10];
unsigned int f[maxn + 10][maxn + 10], g[maxn + 10][maxn + 10];

void init(const unsigned int n)
{
    copy(a + 1, a + n + 1, a + n + 1);
    partial_sum(a, a + n * 2 + 1, sum);
    for (auto& i : f)
        fill(i, i + n * 2 + 1, inf);
    for (unsigned int i = 0; i <= n * 2; ++i)
        f[i][i] = 0;
}
void dp(const unsigned int n)
{
    for (unsigned int i = 1; i < n * 2; ++i)
        for (unsigned int j = 1; j + i <= n * 2; ++j)
            for (unsigned int k = j; k < j + i; ++k)
            {
                const auto val = [&i, &j, &k](const unsigned int d[maxn + 10][maxn + 10]) -> unsigned int {
                    return d[j][k] + d[k + 1][j + i] + sum[j + i] - sum[j - 1];
                };
                f[j][j + i] = min(f[j][j + i], val(f));
                g[j][j + i] = max(g[j][j + i], val(g));
            }
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
    dp(n);
    unsigned int gt = 0, lt = inf;
    for (unsigned int i = 1; i < n; ++i)
    {
        gt = max(gt, g[i][i + n - 1]);
        lt = min(lt, f[i][i + n - 1]);
    }
    cout << lt << endl
         << gt << endl;
    return 0;
}