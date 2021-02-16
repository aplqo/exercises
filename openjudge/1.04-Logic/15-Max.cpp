#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int a, b, c, ans;
  cin >> a >> b >> c;
  ans = (a > b) ? a : b;
  ans = ((ans > c) ? ans : c);
  cout << ans;
  return 0;
}
