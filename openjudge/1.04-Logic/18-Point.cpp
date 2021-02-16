#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int x, y;
  cin >> x >> y;
  if ((x >= -1) && (x <= 1) && (y >= -1) && (y <= 1))
    cout << "yes";
  else
    cout << "no";
  return 0;
}
