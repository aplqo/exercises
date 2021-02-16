#include <iostream>
using namespace std;
int main()
{
  double n, x = 2, s = 0;
  int i = 0;
  cin >> n;
  while (s < n) {
    i++;
    s += x;
    x *= 0.98;
  }
  cout << i;
  return 0;
}
