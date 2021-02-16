#include <iostream>
using namespace std;
int main()
{
  int n;
  cin >> n;
  int s, z;
  int ans = 0, t = 0;
  for (int i = 0; i < n; i++) {
    cin >> s >> z;
    if ((s >= 90 && s <= 140) && (z >= 60 && z <= 90)) {
      t++;
    }
    else {
      ans = (ans > t ? ans : t);
      t = 0;
    }
  }
  ans = ans > t ? ans : t;
  cout << ans;
  return 0;
}
