#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#define rep(v) for (int v = 1; v <= n; ++v)
using namespace std;
const int maxn = 15;

const int dx[2] = { 0, -1 }, dy[2] = { -1, 0 };
int dat[maxn][maxn], ans[maxn][maxn][maxn][maxn];

int main()
{
    int n;
    ios_base::sync_with_stdio(false);
    cin >> n;
    while (true)
    {
        int x, y, v;
        cin >> x >> y >> v;
        if (x == 0 && y == 0 && v == 0)
            break;
        dat[x][y] = v;
    }
    rep(x1)
        rep(y1)
            rep(x2)
                rep(y2)
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
    cout << ans[n][n][n][n];
    return 0;
}