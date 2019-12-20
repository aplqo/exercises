#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
const int maxn = 1005;

const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int ans[maxn * maxn], n, m, vis[maxn][maxn];
bool maz[maxn][maxn];
struct node
{
    node(int nx, int ny) : x(nx), y(ny){};
    int x, y;
};
queue<node> q;

void bfs(node beg, int id)
{
    q.push(beg);
    vis[beg.x][beg.y] = id;
    ans[id] = 1;
    while (q.size())
    {
        node cur = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i)
        {
            int nx = cur.x + dx[i], ny = cur.y + dy[i];
            if (nx < 1 || ny < 1 || nx > n || ny > n)
                continue;
            if (maz[nx][ny] ^ maz[cur.x][cur.y])
            {
                if (vis[nx][ny])
                    continue;
                ++ans[id];
                vis[nx][ny] = id;
                q.push(node(nx, ny));
            }
        }
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        for (int j = 1; j <= n; ++j)
            maz[i][j] = cin.get() - '0';
    }
    for (int i = 1; i <= m; ++i)
    {
        int x, y;
        cin >> x >> y;
        if (!vis[x][y])
            bfs(node(x, y), i);
        cout << ans[vis[x][y]] << endl;
    }
    return 0;
}
