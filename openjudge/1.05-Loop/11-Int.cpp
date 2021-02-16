#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main()
{
  int n, t, one = 0, five = 0, ten = 0;
  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> t;
    switch (t) {
      case 1:
        one++;
        break;
      case 5:
        five++;
        break;
      case 10:
        ten++;
        break;
    }
  }
  cout << one << endl << five << endl << ten;
  return 0;
}
