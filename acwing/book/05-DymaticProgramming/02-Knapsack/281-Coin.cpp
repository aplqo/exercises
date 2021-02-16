#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdlib>
#include <iostream>
using namespace std;
const int maxn = 100, maxm = 1e5;

unsigned int a[maxn + 10], c[maxn + 10];
unsigned int minv[maxm + 10];
bool f[maxm + 10] = {true};

void dp(const unsigned int n, const unsigned int m)
{
  for (unsigned int i = 0; i < n; ++i) {
    fill(minv, minv + m + 1, 0);
    for (unsigned int j = a[i]; j <= m; ++j)
      if (!f[j] && f[j - a[i]] && minv[j - a[i]] < c[i]) {
        f[j] = true;
        minv[j] = minv[j - a[i]] + 1;
      }
  }
}
void solve()
{
  static const auto read = [](unsigned int& i) -> void { cin >> i; };
  unsigned int n, m;
  cin >> n >> m;
  if (!n && !m) exit(0);
  for_each(a, a + n, read);
  for_each(c, c + n, read);
  dp(n, m);
  cout << count(f + 1, f + 1 + m, true) << endl;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  while (true) {
    solve();
    fill(f + 1, f + maxm + 1, false);
  }
  return 0;
}