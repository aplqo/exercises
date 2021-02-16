#include <iostream>
#include <limits>
#include <queue>
using namespace std;
const int maxn = 1005;

const int dx[] = {1, 0, -1, 0}, dy[] = {0, 1, 0, -1};
struct node {
  node(int x, int y) : x(x), y(y){};
  int x;
  int y;
};
queue<node> qu;
int dat[maxn][maxn], ans;

void bfs(node beg)
{
  qu.push(beg);
  while (qu.size()) {
    node t = qu.front();
    qu.pop();
    dat[t.x][t.y] = 0;
    for (int i = 0; i < 4; ++i) {
      int nx = t.x + dx[i], ny = t.y + dy[i];
      if (!dat[nx][ny]) continue;
      qu.push(node(nx, ny));
    }
  }
}
int main()
{
  ios_base::sync_with_stdio(false);
  int m, n;
  cin >> m >> n;
  for (int i = 1; i <= m; ++i) {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int j = 1; j <= n; ++j)
      dat[j][i] = cin.get() - '0';
  }
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
      if (dat[i][j]) {
        bfs(node(i, j));
        ++ans;
      }
  cout << ans;
  return 0;
}