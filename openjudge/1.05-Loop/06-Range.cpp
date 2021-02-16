#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  int n, t, max = 0, min = 1000, ans;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &t);
    max = (max > t ? max : t);
    min = (min < t ? min : t);
  }
  ans = max - min;
  printf("%d", ans);
  return 0;
}
