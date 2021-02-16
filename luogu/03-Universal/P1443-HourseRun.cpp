#include <algorithm>
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 405;

const int dx[8] = {1, 1, 2, 2, -1, -1, -2, -2},
          dy[8] = {2, -2, 1, -1, 2, -2, 1, -1};
int ans[maxn][maxn], m, n;
bool vis[maxn][maxn];
struct point {
  int x, y;
} hos;
queue<point> q;

int main()
{
  cin >> m >> n;
  for_each(ans + 1, ans + m + 1,
           [](int(&i)[maxn]) -> void { fill(i + 1, i + n + 1, -1); });
  cin >> hos.x >> hos.y;
  ans[hos.x][hos.y] = 0;
  vis[hos.x][hos.y] = true;
  q.push(hos);
  while (q.size()) {
    point npos = q.front();
    q.pop();
    int dn = ans[npos.x][npos.y];
    for (int i = 0; i < 8; ++i) {
      int nx = npos.x + dx[i], ny = npos.y + dy[i];
      if (nx < 1 || ny < 1 || nx > m || ny > n) continue;
      if (vis[nx][ny]) continue;
      vis[nx][ny] = true;
      ans[nx][ny] = dn + 1;
      q.push({nx, ny});
    }
  }
  cout.setf(ios::left);
  for (int i = 1; i <= m; ++i) {
    for (int j = 1; j <= n; ++j) {
      cout.width(5);
      cout << ans[i][j];
    }
    cout << endl;
  }
  return 0;
}
