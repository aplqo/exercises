#include <iostream>
using namespace std;
int main()
{
  int n;
  long long ans = 0;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    bool flag = true;
    if (i % 7 == 0) continue;
    int x = i;
    while (x) {
      if (x % 10 == 7) {
        flag = false;
        break;
      }
      x /= 10;
    }
    if (flag) ans += i * i;
  }
  cout << ans;
  return 0;
}
