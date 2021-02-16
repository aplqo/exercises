#include <cstdio>
#include <iostream>
using std::cin;
using std::printf;
int main()
{
  char a;
  int b;
  float c;
  double d;
  cin >> a >> b >> c >> d;
  printf("%c %d %.6f %.6lf", a, b, c, d);
  return 0;
}
