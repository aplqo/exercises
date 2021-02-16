#include <iostream>
using std::cin;
using std::cout;

int main()
{
  int f;
  bool b;
  cin >> f;
  b = f;
  f = b;
  cout << f;
  cout.flush();
  return 0;
}
