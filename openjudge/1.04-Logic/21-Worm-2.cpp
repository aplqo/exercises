#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int n, x, y;
  cin >> n >> x >> y;
  {
    int t = y / x;
    if (y % x) t++;
    n -= t;
    n = (n >= 0) ? n : 0;
  }
  cout << n;
  return 0;
}
