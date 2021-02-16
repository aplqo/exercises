#include <iostream>
using namespace std;

int cross[129][129], sum[130][130];
int ans, num;

int main()
{
  int d, n;
  auto fix = [](int x) -> int { return x < 1 ? 1 : (x > 129 ? 129 : x); };
  auto gets = [](int x1, int y1, int x2, int y2) -> int {
    return sum[x2][y2] - sum[x2][y1 - 1] - sum[x1 - 1][y2] +
           sum[x1 - 1][y1 - 1];
  };
  cin >> d >> n;
  {
    int x, y, k;
    for (int i = 0; i < n; i++) {
      cin >> x >> y >> k;
      cross[x][y] = k;
    }
  }
  for (int i = 1; i < 130; i++)
    for (int j = 1; j < 130; j++)
      sum[i][j] = sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1] +
                  cross[i - 1][j - 1];
  for (int i = 1; i < 130; i++)
    for (int j = 1; j < 130; j++) {
      int s = gets(fix(i - d), fix(j - d), fix(i + d), fix(j + d));
      if (s > ans) {
        ans = s;
        num = 1;
      }
      else if (s == ans)
        num++;
    }
  cout << num << ' ' << ans;
  return 0;
}