#include <iostream>
using namespace std;
int ans[40][40];

int main()
{
  int n;
  cin >> n;
  int x = (n + 1) / 2, y = 1;
  ans[x][y] = 1;
  ans[n + 1][0] = -1;
  for (int i = 2; i <= n * n; i++) {
    if (ans[x + 1][y - 1] != 0)
      y++;
    else {
      x++;
      y--;
    }
    if (x > n) x = 1;
    if (y > n) y = 1;
    if (x < 1) x = n;
    if (y < 1) y = n;
    ans[x][y] = i;
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++)
      cout << ans[j][i] << ' ';
    cout << endl;
  }
  return 0;
}
