#include <cstdio>
using namespace std;
int main()
{
  int n, t;
  scanf("%d", &n);
  int p = 1, q = 2;
  double ans = 0;
  for (int i = 0; i < n; i++) {
    ans += (double)q / p;
    t = q;
    q += p;
    p = t;
  }
  printf("%.4lf", ans);
  return 0;
}
