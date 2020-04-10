#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 50;

const int dx[2] = { 0, -1 }, dy[2] = { -1, 0 };
int dat[maxn + 10][maxn + 10], ans[maxn + 10][maxn + 10][maxn + 10][maxn + 10];

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    int n, m;
    cin >> m >> n;
    for (int i = 1; i <= m; ++i)
        for (int j = 1; j <= n; ++j)
            cin >> dat[i][j];
    for (int x1 = 1; x1 <= m; ++x1)
        for (int x2 = 1; x2 <= m; ++x2)
            for (int y1 = 1; y1 <= n; ++y1)
                for (int y2 = 1; y2 <= n; ++y2)
                {
                    int& cu = ans[x1][y1][x2][y2];
                    for (int i = 0; i < 2; ++i)
                        for (int j = 0; j < 2; ++j)
                        {
                            int cx1 = x1 + dx[i], cx2 = x2 + dx[j];
                            int cy1 = y1 + dy[i], cy2 = y2 + dy[j];
                            cu = max(cu, ans[cx1][cy1][cx2][cy2]);
                        }
                    cu += dat[x1][y1];
                    if (x1 != x2 || y1 != y2)
                        cu += dat[x2][y2];
                }
    cout << ans[m][n][m][n];
    return 0;
}