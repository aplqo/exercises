#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int a, b;
  cin >> a >> b;
  if ((a >= 10) || (b >= 20))
    cout << 1;
  else
    cout << 0;
  return 0;
}
