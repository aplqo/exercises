#include <algorithm>
#include <iostream>
using namespace std;

struct ap {
  int x, y;
} apples[5000];
int n, h, s;
int main()
{
  int ans = 0;
  {
    int a, b, t, x, y;
    cin >> t >> s;
    cin >> a >> b;
    h = a + b;
    for (int i = 0; i < t; i++) {
      cin >> x >> y;
      if (h >= x) {
        apples[n] = {x, y};
        n++;
      }
    }
  }
  sort(apples, apples + n, [](ap a, ap b) { return a.y < b.y; });
  for (ap* i = apples; i < apples + n; i++) {
    if (i->y > s) break;
    ans++;
    s -= i->y;
  }
  cout << ans;
  return 0;
}
