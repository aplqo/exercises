#include <iostream>
using std::cin;
using std::cout;
const unsigned int price = 19;
int main()
{
  unsigned int c;
  {
    unsigned int a, b;
    cin >> a >> b;
    c = a * 10 + b;
  }
  cout << (c / price);
  cout.flush();
  return 0;
}
