#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  int base, n;
  scanf("%d%d", &base, &n);
  double rat = base;
  for (int i = 0; i < n; i++) {
    rat *= 0.1 / 100 + 1;
  }
  printf("%.4lf", rat);
  return 0;
}
