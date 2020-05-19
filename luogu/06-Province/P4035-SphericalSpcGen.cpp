#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
using namespace std;
const int maxn = 10;
using real = double;

real b[maxn + 1 + 10][maxn + 10];
real equ[maxn + 10][maxn + 1 + 10];

void equation(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j)
        {
            equ[i][j] = b[i][j] - b[i + 1][j];
            equ[i][n] += (b[i][j] + b[i + 1][j]) * (b[i][j] - b[i + 1][j]) / 2;
        }
    }
}
void solve(const unsigned int n)
{
    for (unsigned int i = 0; i < n; ++i)
    {
        if (equ[i][i] >= 0 && equ[i][i] <= 0)
        {
            for (unsigned int j = i + 1; j < n; ++j)
                if (equ[j][i] != 0)
                {
                    swap_ranges(equ[i], equ[i] + n + 1, equ[j]);
                    break;
                }
        }
        for (unsigned int j = i + 1; j < n; ++j)
        {
            if (equ[j][i] >= 0 && equ[j][i] <= 0)
                continue;
            const real rat = equ[j][i] / equ[i][i];
            for (unsigned int k = i; k <= n; ++k)
                equ[j][k] -= equ[i][k] * rat;
        }
    }
    for (unsigned int i = n - 1; i > 0; --i)
    {
        equ[i][n] /= equ[i][i];
        for (unsigned int j = 0; j < i; ++j)
            equ[j][n] -= equ[j][i] * equ[i][n];
    }
    equ[0][n] /= equ[0][0];
}
int main()
{
    unsigned int n;
    scanf("%d", &n);
    for (unsigned int i = 0; i <= n; ++i)
        for (unsigned int j = 0; j < n; ++j)
            scanf("%lf", &b[i][j]);
    equation(n);
    solve(n);
    for (unsigned int i = 0; i < n; ++i)
        printf("%.3lf ", equ[i][n]);
    return 0;
}