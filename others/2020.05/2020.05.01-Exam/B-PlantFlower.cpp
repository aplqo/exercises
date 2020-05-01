#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;
using num_t = long long;
constexpr num_t ninf = numeric_limits<num_t>::lowest();
const int maxn = 3000;

num_t f[maxn + 10][maxn + 10][2], a[maxn + 10];

void init()
{
    num_t *b = &f[0][0][0], *e = b + sizeof(f) / sizeof(num_t);
    fill(b, e, ninf);
}
num_t dp(const unsigned int beg, const unsigned int end, const unsigned int m)
{
    init();
    f[beg][0][0] = 0;
    for (unsigned int i = beg; i <= end; ++i)
        for (unsigned int j = 1; j <= i / 2 + i % 2; ++j)
        {
            f[i][j][0] = max(f[i - 1][j][0], f[i - 1][j][1]);
            f[i][j][1] = f[i - 1][j - 1][0] + a[i];
        }
    return max(f[end][m][0], f[end][m][1]);
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, m;
    cin >> n >> m;
    for (unsigned int i = 1; i <= n; ++i)
        cin >> a[i];
    const auto v = max(dp(1, n - 1, m), dp(2, n, m));
    if (v)
        cout << v << endl;
    else
        cout << "Error!" << endl;
    return 0;
}