#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <cstdio>
using namespace std;
const unsigned int maxn = 300000;
using real = long double;

real p[maxn + 1], e[2][maxn + 1];
char str[maxn + 2];

void dp(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i) {
    e[0][i] = (e[0][i - 1] + 1) * p[i];
    e[1][i] = e[1][i - 1] + (2 * e[0][i - 1] + 1) * p[i];
  }
}
int main()
{
  unsigned int n;
  scanf("%u", &n);
  scanf("%s", str);
  for (unsigned int i = 1; i <= n; ++i)
    switch (str[i - 1]) {
      case 'x':
        p[i] = 0;
        break;
      case 'o':
        p[i] = 1;
        break;
      default:
        p[i] = 0.5;
    }
  dp(n);
  printf("%.4Lf\n", round(e[1][n] * 1e4) / 1e4);
  return 0;
}