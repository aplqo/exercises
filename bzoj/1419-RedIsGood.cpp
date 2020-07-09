#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 5000;

long double dp(const unsigned int r, const unsigned int b)
{
    long double f[2][maxn + 1];
    for (unsigned int i = 1; i <= r; ++i)
    {
        const unsigned int cur = i & 0x01, lst = !cur;
        f[cur][0] = i;
        for (unsigned int j = 1; j <= b; ++j)
            f[cur][j] = max<long double>(((f[lst][j] + 1) * i + (f[cur][j - 1] - 1) * j) / (i + j), 0);
    }
    return static_cast<unsigned long long>(f[r & 0x01][b] * 1e6) / 1e6;
}
int main()
{
    unsigned int r, b;
    cin >> r >> b;
    printf("%.6Lf\n", dp(r, b));
    return 0;
}
