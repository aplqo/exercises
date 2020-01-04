#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 55;

enum class dir : int
{
    W = 0,
    S = 1,
    E = 2,
    N = 3
};
const int dx[4] = { 0, 1, 0, -1 }, dy[4] = { -1, 0, 1, 0 };
struct node
{
    node(int x, int y, int d)
        : x(x)
        , y(y)
        , d(d){};
    int x, y, d;
    inline node turn(int di)
    {
        return node(x, y, (d + di + 4) % 4);
    }
    template <class T>
    inline T& operator[](T dat[maxn][maxn][4])
    {
        return dat[x][y][d];
    }
};
int n, m, bx, by, bd, ex, ey;
bool maz[maxn][maxn], vis[maxn][maxn][4];
int ans[maxn][maxn][4];
queue<node> que;

inline void bfs()
{
    que.push(node(bx, by, bd));
    que.front()[ans] = 0;
    que.front()[vis] = true;
    while (!que.empty())
    {
        node cur = que.front();
        cur[vis] = false;
        que.pop();
        const auto tes = [&cur](node&& t, int dis) -> void {
            if (t[ans] <= cur[ans] + dis)
                return;
            t[ans] = cur[ans] + dis;
            if (!t[vis])
            {
                t[vis] = true;
                que.push(t);
            }
        };
        for (int i = 1; i <= 3; ++i)
        {
            int nx = cur.x + dx[cur.d] * i, ny = cur.y + dy[cur.d] * i;
            if (nx < 1 || ny < 1 || nx >= n || ny >= m || maz[nx][ny])
                break;
            tes(node(nx, ny, cur.d), 1);
        }
        tes(cur.turn(1), 1);
        tes(cur.turn(-1), 1);
    }
}
inline void read()
{
    cin >> n >> m;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= m; ++j)
        {
            int t;
            cin >> t;
            if (t)
            {
                maz[i][j] = true;
                maz[i - 1][j - 1] = true;
                maz[i - 1][j] = true;
                maz[i][j - 1] = true;
            }
        }
    cin >> bx >> by >> ex >> ey;
    {
        char d;
        dir nd;
        cin >> d;
        switch (d)
        {
        case 'E':
            nd = dir::E;
            break;
        case 'W':
            nd = dir::W;
            break;
        case 'S':
            nd = dir::S;
            break;
        case 'N':
            nd = dir::N;
            break;
        }
        bd = int(nd);
    }
    for (int i = 0; i <= n; ++i)
        for_each(ans[i], ans[i] + 1 + m, [](int(&t)[4]) -> void { fill(t, t + 4, INT_MAX); });
}

int main()
{
    ios_base::sync_with_stdio(false);
    read();
    bfs();
    int a = *min_element(ans[ex][ey], ans[ex][ey] + 4);
    if (a != INT_MAX)
        cout << a << endl;
    else
        cout << "-1";
    return 0;
}