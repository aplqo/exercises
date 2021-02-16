#include <iostream>
using namespace std;
int main()
{
  int num[100], n, sum[100][100], ans = 0;
  cin >> n;
  for (int i = 0; i < n; i++)
    cin >> num[i];
  for (int i = 0; i < n; i++)
    for (int j = i + 1; j < n; j++)
      sum[i][j] = num[i] + num[j];
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++)
      for (int k = j + 1; k < n; k++)
        if (sum[j][k] == num[i]) {
          ans++;
          goto s;
        }
  s:;
  }
  cout << ans;
  return 0;
}
