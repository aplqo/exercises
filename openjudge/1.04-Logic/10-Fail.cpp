#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int a, b;
  cin >> a >> b;
  if ((a >= 60) ^ (b >= 60))
    cout << 1;
  else
    cout << 0;
  return 0;
}
