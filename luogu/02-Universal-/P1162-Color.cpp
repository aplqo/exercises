#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 35;

const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
struct node
{
    node(int nx, int ny) : x(nx), y(ny){};
    int x, y;
    inline int &val(int dat[maxn][maxn])
    {
        return dat[x][y];
    }
    inline node transform(int d)
    {
        return node(x + dx[d], y + dy[d]);
    }
};
queue<node> que;
int vis[maxn][maxn], dat[maxn][maxn];
int n;

bool bfs(node &&beg, int id)
{
    bool f = true;
    beg.val(vis) = id;
    que.push(beg);
    while (que.size())
    {
        node cur = que.front();
        que.pop();
        for (int i = 0; i < 4; ++i)
        {
            node t = cur.transform(i);
            if (t.x < 0 || t.x >= n || t.y < 0 || t.y >= n)
            {
                f = false;
                continue;
            }
            if (t.val(vis) || t.val(dat) == 1)
                continue;
            t.val(vis) = id;
            que.push(t);
        }
    }
    return f;
}
int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> dat[i][j];
    int t = 0;
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
        {
            if (dat[i][j] == 1)
                continue;
            if (!vis[i][j])
                if (bfs(node(i, j), ++t))
                    goto e;
        }
e:;
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
            if (vis[i][j] == t)
                cout << "2"
                     << " ";
            else
                cout << dat[i][j] << " ";
        cout << endl;
    }
    return 0;
}