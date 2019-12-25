#ifdef APDEBUG
#define APINPROG
#define COLOR
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 505;

const int dx[] = { 1, 0, -1, 0 }, dy[] = { 0, 1, 0, -1 };
int ans[maxn][maxn], n, m;
struct node
{
    node(int nx, int ny)
        : x(nx)
        , y(ny){};
    int x, y;
    inline int& operator[](int dat[maxn][maxn])
    {
        return dat[x][y];
    }
};
queue<node> que;
inline void bfs(node&& beg)
{
    que.push(beg);
    beg[ans] = 1;
    while (!que.empty())
    {
        node cur = que.front();
        que.pop();
        for (int i = 0; i < 4; ++i)
        {
            node t(cur.x + dx[i], cur.y + dy[i]);
            if (t.x < 1 || t.y < 1 || t.x > n || t.y > m)
                continue;
            if (t[ans] <= cur[ans] + 1)
                continue;
            t[ans] = cur[ans] + 1;
            que.push(t);
        }
    }
}
int main()
{
    int a, b;
    ios_base::sync_with_stdio(false);
    cin >> n >> m >> a >> b;
    for_each(ans, ans + n + 1, [](int(&i)[maxn]) -> void { fill(i, i + m + 1, INT_MAX); });
    for (int i = 0; i < a; ++i)
    {
        int x, y;
        cin >> x >> y;
        bfs(node(x, y));
    }
    for (int i = 0; i < b; ++i)
    {
        int x, y;
        cin >> x >> y;
        cout << (ans[x][y] - 1) << endl;
    }
    return 0;
}
