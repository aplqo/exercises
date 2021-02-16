#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
using namespace std;

enum { A = 0, B = 1, C = 2 };
const int price[3] = {7, 4, 3};
int val[3], ans = 0;

int exgcd(const int a, const int b, int& x, int& y)
{
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  int g = exgcd(b, a % b, x, y);
  int t = x;
  x = y;
  y = t - (a / b) * y;
  return g;
}
inline bool fun(const int n, const int a, const int b)  // a>b
{
  int x0, y0, res;
  {
    const int g = exgcd(price[a], price[b], x0, y0);
    if (n % g) return false;
    x0 *= n / g;
    y0 *= n / g;
  }
  const int tmin = -y0 / price[a] + (y0 % price[a] ? 1 : 0),
            tmax = x0 / price[b];
  if (tmax <= tmin) return false;
  res = x0 + y0 + (price[a] - price[b]) * tmax;
  if (res > ans) {
    ans = res;
    val[A] = val[B] = val[C] = 0;
    val[a] = x0 - price[b] * tmax;
    val[b] = y0 + price[a] * tmax;
  }
  return true;
}
int main()
{
#if (!defined(APTEST)) && (!defined(APDEBUG))
  freopen("order.in", "r", stdin);
  freopen("order.out", "w", stdout);
#endif
  int n;
  scanf("%d", &n);
  if (!(n % 14)) {
    printf("%d %d %d\n", n / 14, n / 14, n / 14);
    return 0;
  }
  int l = 0, r = n / 14;
  while (l <= r) {
    int m = (l + r) >> 1, lst = n - 14 * m;
    if (fun(lst, A, B) || fun(lst, B, C) || fun(lst, A, C))
      l = m + 1;
    else
      r = m - 1;
  }
  if (r < 0)
    printf("-1\n");
  else {
    const int lst = n - 14 * r;
    ans = 0;
    fun(lst, A, B);
    fun(lst, B, C);
    fun(lst, A, C);
    printf("%d %d %d\n", val[A] + r, val[B] + r, val[C] + r);
  }
  return 0;
}