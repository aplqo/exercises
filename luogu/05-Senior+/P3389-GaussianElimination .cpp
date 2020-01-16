#ifdef APDEBUG
#define COLOR
#define APINPROG
#undef _MSC_VER
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iostream>
using namespace std;
const int maxn = 105;
typedef double fType;

fType matrix[maxn][maxn];
int n;

void gauss()
{
    for (int i = 0; i < n; ++i)
    {
        if (!matrix[i][i])
        {
            for (int k = i + 1; k < n; ++k)
                if (matrix[k][i])
                {
                    swap_ranges(matrix[k], matrix[k] + n + 1, matrix[i]);
                    break;
                }
        }
        for (int j = i + 1; j < n; ++j)
        {
            fType val = matrix[j][i] / matrix[i][i];
            for (int k = i; k <= n; ++k)
                matrix[j][k] -= val * matrix[i][k];
        }
    }
    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = n - 1; j > i; --j)
            matrix[i][n] -= matrix[i][j] * matrix[j][n];
        if (!matrix[i][i])
        {
            cout << "No Solution" << endl;
            exit(0);
        }
        matrix[i][n] /= matrix[i][i];
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j <= n; ++j)
            cin >> matrix[i][j];
    gauss();
    for (int i = 0; i < n; ++i)
        printf("%.2lf\n", matrix[i][n]);
    return 0;
}