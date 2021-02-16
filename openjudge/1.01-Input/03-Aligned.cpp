#include <iostream>
using std::cin;
using std::cout;
int main()
{
  int dat[3];
  for (auto& i : dat) {
    cin >> i;
  }
  for (const auto& i : dat) {
    cout.width(8);
    cout << i;
    cout << " ";
  }
  cout.flush();
  return 0;
}
