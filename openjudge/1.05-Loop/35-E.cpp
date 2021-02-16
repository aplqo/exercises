#include <cstdio>
using namespace std;
int main()
{
  int n;
  double ans = 1, last = 1;
  scanf("%d", &n);
  for (int i = 1; i <= n; i++) {
    last *= i;
    ans += 1.0 / last;
  }
  printf("%.10lf", ans);
  return 0;
}
