#include <iostream>
using namespace std;
int main()
{
  int k;
  cin >> k;
  int d = 1, ans = 0, t = 0;
  for (int i = 0; i < k; i++, t++) {
    if (t >= d) {
      d++;
      t = 0;
    }
    ans += d;
  }
  cout << ans;
  return 0;
}
