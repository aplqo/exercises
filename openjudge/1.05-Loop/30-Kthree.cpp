#include <iostream>
using namespace std;
int main()
{
  int m, k, t = 0;
  cin >> m >> k;
  if (m % 19) {
    cout << "NO";
    return 0;
  }
  while (m) {
    if (m % 10 == 3) t++;
    m /= 10;
  }
  if (t == k)
    cout << "YES";
  else
    cout << "NO";
  return 0;
}
