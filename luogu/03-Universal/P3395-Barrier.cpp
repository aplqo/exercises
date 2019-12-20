#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <iostream>
#include <algorithm>
#include <climits>
#include <queue>
using namespace std;
const int maxn = 1005;

const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n = maxn - 2, dat[maxn][maxn], vis[maxn][maxn];
struct node
{
    node(int nx, int ny) : x(nx), y(ny){};
    int &getVal(int d[maxn][maxn])
    {
        return d[x][y];
    }
    node trans(int i)
    {
        return node(x + dx[i], y + dy[i]);
    }
    int x, y;
};
queue<node> que;

void solve()
{
    cin >> n;
    for (int i = 1; i <= 2 * n - 2; ++i)
    {
        int x, y;
        cin >> x >> y;
        dat[x][y] = i + 1;
    }
    if (n == 1)
    {
        cout << "Yes" << endl;
        return;
    }
    que.push(node(1, 1));
    que.front().getVal(vis) = 1;
    while (!que.empty())
    {
        node cur = que.front();
        que.pop();
        for (int i = 0; i < 4; ++i)
        {
            node t = cur.trans(i);
            if (t.x < 1 || t.y < 1 || t.x > n || t.y > n)
                continue;
            if (t.getVal(dat) <= cur.getVal(vis) || t.getVal(vis))
                continue;
            t.getVal(vis) = cur.getVal(vis) + 1;
            if (t.x == n && t.y == n)
            {
                cout << "Yes" << endl;
                que.~queue();
                new (&que) queue<node>;
                return;
            }
            que.push(t);
        }
    }
    cout << "No" << endl;
    return;
}
inline void fil(int t[maxn][maxn], int val)
{
    for_each(t + 1, t + n + 1, [&val](int(&i)[maxn]) -> void { fill(i + 1, i + n + 1, val); });
}

int main()
{
    ios_base::sync_with_stdio(false);
    int t;
    cin >> t;
    for (int i = 0; i < t; ++i)
    {
        fil(dat, INT_MAX);
        solve();
        fil(vis, 0);
    }
    return 0;
}