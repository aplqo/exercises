#include <cstdio>
using std::printf;
using std::scanf;
int main()
{
  int tg = 0, ts = 0, tc = 0;
  int g, s, c;
  int n;
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d%d%d", &g, &s, &c);
    tg += g;
    ts += s;
    tc += c;
  }
  printf("%d %d %d %d", tg, ts, tc, tg + ts + tc);
  return 0;
}
