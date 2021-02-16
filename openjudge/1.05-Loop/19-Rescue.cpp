#include <cmath>
#include <iostream>
using std::ceil;
using std::cin;
using std::cout;
using std::sqrt;
int main()
{
  int n;
  double ans = 0, dist;
  cin >> n;
  for (int i = 0; i < n; i++) {
    double x, y, p;
    cin >> x >> y >> p;
    dist = sqrt(x * x + y * y);
    ans += dist / 25 + 1.5 * p;
  }
  cout << ceil(ans);
  return 0;
}
