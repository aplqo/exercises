#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  int r, m, y;
  scanf("%d%d%d", &r, &m, &y);
  double d = r / 100.0 + 1;
  double ans = m;
  for (int i = 0; i < y; i++)
    ans *= d;
  printf("%d", ((int)ans));
  return 0;
}
