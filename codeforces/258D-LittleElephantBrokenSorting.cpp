#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 3000, maxq = 3000;
typedef long double Real;

struct Operation {
  unsigned int x, y;
} op[maxq + 10];
unsigned int a[maxn + 10];

Real dp(const unsigned int n, const unsigned int q)
{
  static Real f[maxn + 10][maxn + 10];
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j)
      f[i][j] = a[i] > a[j];
  for (const Operation* i = op; i < op + q; ++i) {
    for (unsigned int j = 0; j < n; ++j)
      if (j != i->x && j != i->y) {
        f[j][i->x] = f[j][i->y] = (f[j][i->x] + f[j][i->y]) / 2;
        f[i->x][j] = f[i->y][j] = (f[i->x][j] + f[i->y][j]) / 2;
      }
    f[i->x][i->x] = f[i->y][i->y] = 0;
    f[i->x][i->y] = f[i->y][i->x] = (f[i->x][i->y] + f[i->y][i->x]) / 2;
  }
  Real ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = i + 1; j < n; ++j)
      ret += f[i][j];
  return ret;
}

int main()
{
#ifndef APTEST
  ios::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  for (Operation* i = op; i < op + q; ++i) {
    cin >> i->x >> i->y;
    --(i->x);
    --(i->y);
    if (i->x > i->y) swap(i->x, i->y);
  }
  cout << fixed << setprecision(10) << dp(n, q) << "\n";
  return 0;
}