#include <iostream>
using std::cin;
using std::cout;

int main()
{
  int a, i;
  double b = 0;
  cin >> a;
  for (i = 1; b <= a; i++) {
    b += 1.0 / i;
  }
  cout << (i - 1);
  cout.flush();
  return 0;
}
