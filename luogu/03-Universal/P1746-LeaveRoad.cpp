#ifdef APDEBUG
#define APINPROG
#include "time.cpp"
#endif
#include <iostream>
#include <limits>
using namespace std;
const int maxn = 1005;

const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
int n;
bool dat[maxn][maxn];
struct node
{
    node() = default;
    node(int nx, int ny) : x(nx), y(ny), ans(0){};
    bool operator==(const node &l)
    {
        return l.x == x && l.y == y;
    }
    inline bool &val()
    {
        return dat[x][y];
    }
    int x, y;
    int ans = 0;
} beg, en;
node que[maxn * maxn], *front = que, *rear = que + 1;

int main()
{
    ios_base::sync_with_stdio(false);
    cin >> n;
    for (int i = 1; i <= n; ++i)
    {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        for (int j = 1; j <= n; ++j)
            dat[i][j] = cin.get() - '0';
    }
    cin >> beg.x >> beg.y >> en.x >> en.y;
    beg.val() = true;
    *front = beg;
    while (front < rear)
    {
        node *cur = front;
        ++front;
        for (int i = 0; i < 4; ++i)
        {
            int nx = cur->x + dx[i], ny = cur->y + dy[i];
            if (nx < 1 || ny < 1 || nx > n || ny > n || dat[nx][ny])
                continue;
            *rear = node(nx, ny);
            rear->ans = cur->ans + 1;
            rear->val() = true;
            if (*rear == en)
            {
                cout << rear->ans << endl;
                return 0;
            }
            ++rear;
        }
    }
    return 0;
}