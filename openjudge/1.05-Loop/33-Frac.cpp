#include <cstdio>
using namespace std;
int main()
{
  int n, t = 1;
  scanf("%d", &n);
  double ans = 0;
  for (int i = 1; i <= n; i++) {
    ans += t * 1.0 / i;
    t *= -1;
  }
  printf("%.4lf", ans);
  return 0;
}
