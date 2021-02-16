#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 80, maxb = 20, maxsb = maxn * maxb;
const unsigned int inf = UINT_MAX / 2;

struct Box {
  unsigned int a, b;
} bx[maxn + 10];
unsigned int ans[maxn + 10];

static void dp(const unsigned int n)
{
  static unsigned int f[maxn + 10][maxn + 10][maxsb + 10];
  fill(reinterpret_cast<unsigned int*>(f),
       reinterpret_cast<unsigned int*>(f) + sizeof(f) / sizeof(unsigned int),
       inf);
  f[0][0][0] = 0;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 0; j <= i; ++j)
      for (unsigned int k = 0; k <= n * maxb; ++k) {
        f[i][j][k] = f[i - 1][j][k] + bx[i].a;
        if (j && k >= bx[i].b)
          f[i][j][k] = min(f[i][j][k], f[i - 1][j - 1][k - bx[i].b]);
      }
  unsigned int sa = 0;
  for (const Box* i = bx + 1; i < bx + 1 + n; ++i)
    sa += i->a;
  for (unsigned int i = 0; i <= n; ++i)
    ans[i] = *min_element(f[n][i] + sa, f[n][i] + maxsb);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  for (Box* i = bx + 1; i < bx + 1 + n; ++i)
    cin >> i->a;
  for (Box* i = bx + 1; i < bx + 1 + n; ++i)
    cin >> i->b;
  dp(n);
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int k;
    cin >> k;
    if (ans[k] < inf)
      cout << ans[k] << "\n";
    else
      cout << "-1\n";
  }
  return 0;
}