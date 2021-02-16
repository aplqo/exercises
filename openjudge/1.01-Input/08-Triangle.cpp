#include <iostream>
using std::cin;
using std::cout;
using std::endl;
int main()
{
  char c;
  cin >> c;
  {
    cout << "  " << c << "  " << endl;
    cout << " " << c << c << c << " " << endl;
    cout << c << c << c << c << c;
  }
  cout.flush();
  return 0;
}
