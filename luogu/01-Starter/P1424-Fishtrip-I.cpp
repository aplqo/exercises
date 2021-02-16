#include <iostream>
using namespace std;
int main()
{
  int x, n, ans = 0;
  cin >> x >> n;
  for (int i = x; i < x + n; i++) {
    if (i % 7 != 0 && i % 7 != 6) ans += 250;
  }
  cout << ans;
  return 0;
}
