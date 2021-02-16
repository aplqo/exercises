#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  int a, b;
  scanf("%d %d", &a, &b);
  float x = (float)b / a;
  x *= 100;
  printf("%.3f%%", x);
  return 0;
}
