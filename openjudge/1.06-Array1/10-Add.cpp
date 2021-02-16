#include <iostream>
using namespace std;
int a[400], b[400], ans[400];
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
  int la = Get(a), lb = Get(b), lc = 399;
  int c = 0;
  for (int *m = (la > lb) ? &la : &lb; (*m) >= 199; la--, lb--, lc--) {
    int s = a[la] + b[lb] + c;
    ans[lc] = s % 10;
    c = s / 10;
  }
  for (; lc < 399; lc++)
    if (ans[lc] != 0) break;
  for (; lc < 400; lc++)
    cout << ans[lc];
  return 0;
}
