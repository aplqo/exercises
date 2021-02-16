#include <iostream>
using namespace std;
int ans[100], t = 99;
int main()
{
  int n;
  cin >> n;
  ans[t] = 1;
  for (int i = 0; i < n; i++) {
    int c = 0, s = 0;
    for (int j = 99; j >= 0; j--) {
      c = ans[j] * 2 + c;
      ans[j] = c % 10;
      c /= 10;
    }
    if (ans[t - 1]) t--;
  }
  for (int i = t; i < 100; i++)
    cout << ans[i];
  return 0;
}
