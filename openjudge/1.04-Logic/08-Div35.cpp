#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int a;
  cin >> a;
  if ((a % 3) || (a % 5))
    cout << "NO";
  else
    cout << "YES";
  return 0;
}
