/*
 * Luogu team T129207: Three kingdom 1v1
 */
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
using namespace std;
using num = long long;
constexpr num inf = numeric_limits<num>::max() >> 1;
const unsigned int maxn = 2000, maxm = 10000;

num s[maxm + 10];
num f[3][maxm + 10];

void init(const unsigned int n, const unsigned int m)
{
  sort(s + 1, s + 1 + m, greater<num>());
  for (auto& i : f)
    fill(i, i + m + 1, inf);
}
num dp(const unsigned int n, const unsigned int m)
{
  auto val = [](num a, num b) -> num { return (a - b) * (a - b); };
  f[0][0] = 0;
  unsigned int p = 1;
  for (unsigned int i = 1; i <= m; ++i, p = (p + 1) % 3) {
    const unsigned int lst1 = (p + 3 - 1) % 3, lst2 = (p + 3 - 2) % 3;
    for (unsigned int j = 0; j <= min(i, m - n * 2); ++j) {
      f[p][j] = j ? f[lst1][j - 1] : inf;
      if (i - 2 >= j && (i - 2 - j) / 2 + 1 <= j)
        f[p][j] = min(f[p][j], f[lst2][j] + val(s[i - 1], s[i]));
    }
  }
  p = (p + 3 - 1) % 3;
  return f[p][m - n * 2];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for_each(s + 1, s + 1 + m, [](num& i) { cin >> i; });
  init(n, m);
  cout << dp(n, m) << endl;
  return 0;
}