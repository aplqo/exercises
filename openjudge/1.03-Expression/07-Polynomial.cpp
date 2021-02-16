#include <cmath>
#include <cstdio>
using std::pow;
using std::printf;
using std::scanf;
int main()
{
  double x, a, b, c, d;
  scanf("%lf%lf%lf%lf%lf", &x, &a, &b, &c, &d);
  double res = a * pow(x, 3) + b * pow(x, 2) + c * pow(x, 1) + d;
  printf("%.7lf", res);
}
