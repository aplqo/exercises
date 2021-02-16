#include <iostream>
using std::cin;
using std::cout;

int main()
{
  float f;
  cin >> f;
  cout << static_cast<int>(f);
  cout.flush();
  return 0;
}
