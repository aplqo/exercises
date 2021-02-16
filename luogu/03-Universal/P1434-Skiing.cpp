#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 100;

const int dx[] = {0, 1, 0, -1}, dy[] = {1, 0, -1, 0};
int r, c, a[maxn][maxn], s[maxn][maxn];

int fun(int x, int y)
{
  if (s[x][y]) return s[x][y];
  s[x][y] = 1;
  for (int i = 0; i < 4; i++) {
    int x2 = x + dx[i], y2 = y + dy[i];
    if (x2 < 0 || y2 < 0 || x2 >= r || y2 >= c) continue;
    if (a[x2][y2] >= a[x][y]) continue;
    s[x][y] = max(fun(x2, y2) + 1, s[x][y]);
  }
  return s[x][y];
}
int main()
{
  int ans = 0;
  cin >> r >> c;
  for (int i = 0; i < r; i++)
    for (int j = 0; j < c; j++)
      cin >> a[i][j];
  for (int i = 0; i < r; i++)
    for (int j = 0; j < c; j++)
      ans = max(ans, fun(i, j));
  cout << ans;
  return 0;
}