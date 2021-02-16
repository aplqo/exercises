#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
typedef int num;
const num ninf = INT_MIN;
const int maxn = 3830;

num f[2][maxn + 10][2], u[maxn + 10];

void init(const unsigned int n)
{
  for (unsigned int i = 0; i < 2; ++i) {
    for (unsigned int j = 1; j <= n; ++j)
      f[i][j][0] = f[i][j][1] = ninf;
    f[i][0][1] = ninf;
  }
  f[0][1][1] = 0;
}
void init2(const unsigned int n)
{
  init(n);
  f[0][1][1] = u[1];
}
num dp(const unsigned int n, const unsigned int b)
{
  unsigned lst1 = 0, cur = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    for (unsigned int j = 1; j <= min(b, i); ++j) {
      f[cur][j][0] = max(f[lst1][j][0], f[lst1][j][1]);
      f[cur][j][1] = max(f[lst1][j - 1][1] + u[i], f[lst1][j - 1][0]);
    }
    lst1 = cur;
    cur = (cur + 1) % 2;
  }
  return max(f[lst1][b][0], f[lst1][b][1]);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, b;
  num ans;
  cin >> n >> b;
  for (num* i = u + 1; i < u + 1 + n; ++i)
    cin >> *i;
  init(n);
  ans = dp(n, b);
  init2(n);
  dp(n, b);
  ans = max(ans, f[(n + 1) % 2][b][1]);
  cout << ans << endl;
  return 0;
}