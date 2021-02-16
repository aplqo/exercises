#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int dat[3];
  for (auto& i : dat) {
    cin >> i;
  }
  cout << dat[1];
  cout.flush();
  return 0;
}
