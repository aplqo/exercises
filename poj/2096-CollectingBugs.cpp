#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cstdio>
using namespace std;
const int maxn = 1000;
typedef long double res_t;
const char res_fmt[] = "%.4llf\n";

res_t f[maxn + 10][maxn + 10];

int main()
{
    unsigned int s, n;
    scanf("%u%u", &s, &n);
    for (unsigned int i = s; i; --i)
        for (unsigned int j = n; j; --j)
        {
            if (i == s && j == n)
            {
                f[i][j] = 1;
                continue;
            }
            f[i][j] = (s - i) * j * f[i + 1][j] + i * (n - j) * f[i][j + 1] + (s - i) * (n - j) * f[i + 1][j + 1] + s * n;
            f[i][j] /= s * n > i * j ? s * n - i * j : 1;
        }
    printf(res_fmt, f[1][1]);
    return 0;
}
