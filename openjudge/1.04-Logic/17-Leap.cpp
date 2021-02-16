#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int a;
  cin >> a;
  bool t =
      (a % 4 == 0) && ((a % 100 != 0) || (a % 400 == 0)) && (a % 3200 != 0);
  if (t)
    cout << "Y";
  else
    cout << "N";
  return 0;
}
