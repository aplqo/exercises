#include <iostream>
using namespace std;

int main()
{
  unsigned long long a, b;
  cin >> a >> b;
  /*
      ax+by=c
      x<b,y<0 or x<0,y<a
      c_max=a(b-1)+b*(-1)=ab-a-b
  */
  cout << (a * b - a - b);
  return 0;
}