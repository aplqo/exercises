#include <iostream>
using namespace std;
const int maxn = 6;

const int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1};
int n, m, ans;
int fx, fy, sx, sy, t;
bool vis[maxn][maxn];

void dfs(int x, int y)
{
  vis[x][y] = true;
  if (x == fx && y == fy) {
    ans++;
    goto e;
  }
  for (int i = 0; i < 4; i++) {
    int nx = x + dx[i], ny = y + dy[i];
    if (vis[nx][ny]) continue;
    if (nx < 1 || ny < 1 || nx > n || ny > m) continue;
    dfs(nx, ny);
  }
e:
  vis[x][y] = false;
}

int main()
{
  cin >> m >> n >> t;
  cin >> sx >> sy >> fx >> fy;
  for (int i = 0; i < t; i++) {
    int x, y;
    cin >> x >> y;
    vis[x][y] = true;
  }
  dfs(sx, sy);
  cout << ans;
  return 0;
}
