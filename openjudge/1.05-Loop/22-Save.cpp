#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int a = 0, save = 0, t;
  for (int i = 0; i < 12; i++) {
    a += 300;
    cin >> t;
    if (a < t) {
      cout << "-" << i + 1;
      return 0;
    }
    a -= t;
    save += (a / 100) * 100;
    a %= 100;
  }
  cout << (double)save * 1.2 + a;
  return 0;
}
