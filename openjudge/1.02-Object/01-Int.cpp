#include <iostream>
using std::cout;

constexpr unsigned int sInt = sizeof(int);
constexpr unsigned int sShort = sizeof(short);

int main()
{
  cout << sInt << " " << sShort;
  cout.flush();
  return 0;
}
