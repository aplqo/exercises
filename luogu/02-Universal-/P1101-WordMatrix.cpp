#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 105;

const char dat[8] = "yizhong";
const int dx[] = { 1, 0, -1, 0, 1, 1, -1, -1 }, dy[] = { 0, 1, 0, -1, 1, 1, -1 };

int n;
char ma[maxn][maxn];
bool vis[maxn][maxn];

void dfs(int x, int y, int di)
{
    const int dix = dx[di], diy = dy[di];
    int nx[8] = { x }, ny[8] = { y };
    for (int i = 1; i < 7; ++i)
    {
        nx[i] = nx[i - 1] + dix;
        ny[i] = ny[i - 1] + diy;
        if (nx[i] < 0 || ny[i] < 0 || nx[i] >= n || ny[i] >= n || ma[nx[i]][ny[i]] != dat[i])
            return;
    }
    for (int i = 0; i < 7; ++i)
        vis[nx[i]][ny[i]] = true;
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n;
    for (int i = 0; i < n; ++i)
        cin >> ma[i];
    for (char(*i)[maxn] = ma; i < ma + n; ++i)
        for_each(*i, *i + n, [](char& t) {if(find(dat,dat+8,t)==dat+8) t='*'; });
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (ma[i][j] == dat[0])
                for (int d = 0; d < 8; ++d)
                    dfs(i, j, d);
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            cout << (vis[i][j] ? ma[i][j] : '*');
        cout << endl;
    }
    return 0;
}