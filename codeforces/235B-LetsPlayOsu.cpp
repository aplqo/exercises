#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <cstdio>
using namespace std;
const unsigned int maxn = 100000;
using real = double;

real p[maxn + 1], e[2][maxn + 1];

void dp(const unsigned int n)
{
    for (unsigned int i = 1; i <= n; ++i)
    {
        e[0][i] = (e[0][i - 1] + 1) * p[i];
        e[1][i] = e[1][i - 1] + (2 * e[0][i - 1] + 1) * p[i];
    }
}
int main()
{
    unsigned int n;
    scanf("%u", &n);
    for (unsigned int i = 1; i <= n; ++i)
        scanf("%lf", p + i);
    dp(n);
    printf("%lf\n", e[1][n]);
    return 0;
}