#include <iostream>
using namespace std;
int main()
{
  int r, t, x, ans = 0;
  cin >> r >> x;
  for (int i = 1; i <= r; i++) {
    t = i;
    while (t) {
      if (t % 10 == x) ans++;
      t /= 10;
    }
  }
  cout << ans;
  return 0;
}
