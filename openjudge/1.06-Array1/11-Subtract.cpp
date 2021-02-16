#include <cstdio>
#include <iostream>
using namespace std;
int a[400], b[400], ans[200];
int Get(int *x)
{
  char t = getchar();
  int v = 200;
  while (t <= '9' && t >= '0') {
    x[v] = t - '0';
    t = getchar();
    v++;
  }
  return v - 1;
}
int main()
{
  int la = Get(a), lb = Get(b), lc = 199;
  int c = 0;
  for (int *m = (la > lb) ? &la : &lb; (*m) > 199; la--, lb--, lc--) {
    int s = a[la] - b[lb] - c;
    if (s < 0) {
      s += 10;
      c = 1;
    }
    else
      c = 0;
    ans[lc] = s;
  }
  for (; lc < 199; lc++)
    if (ans[lc] != 0) break;
  for (; lc < 200; lc++)
    cout << ans[lc];
  return 0;
}
