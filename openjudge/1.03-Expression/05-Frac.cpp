#include <cstdio>
#include <iostream>
using std::cin;
using std::printf;

int main()
{
  int a, b;
  cin >> a >> b;
  printf("%.9lf", (double)a / (double)b);
  return 0;
}
