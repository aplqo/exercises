#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  double a;
  scanf("%lf", &a);
  if ((a >= 0) && (a < 5)) {
    a = -a + 2.5;
  }
  else if ((a >= 5) && (a < 10)) {
    a = 2 - 1.5 * (a - 3) * (a - 3);
  }
  else {
    a = a / 2 - 1.5;
  }
  printf("%.3lf", a);
  return 0;
}
