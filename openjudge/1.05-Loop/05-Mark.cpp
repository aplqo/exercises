#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  int n, t, ans = 0;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &t);
    ans = (ans > t ? ans : t);
  }
  printf("%d", ans);
  return 0;
}
