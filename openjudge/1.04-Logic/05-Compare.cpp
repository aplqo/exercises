#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int a, b;
  cin >> a >> b;
  char c = (a > b ? '>' : '<');
  if (a == b) c = '=';
  cout << c;
  return 0;
}
