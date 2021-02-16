#include <iostream>
using namespace std;
int main()
{
  int a, i;
  double b = 0;
  cin >> a;
  for (i = 1; b <= a; i++) {
    b += 1.0 / i;
  }
  cout << (i - 1);
  return 0;
}
