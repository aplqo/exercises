#include <iostream>
using namespace std;
int main()
{
  int n, t;
  cin >> n;
  while (n) {
    t = n % 10;
    cout << t << ' ';
    n /= 10;
  }
  return 0;
}
