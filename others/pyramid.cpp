#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
using namespace std;
const int maxn = 3000;
constexpr int ninf = numeric_limits<int>::min() / 2;

int w[maxn + 10], ans[maxn + 10];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  int n, m;
  int bas;
  cin >> n >> m;
  cin >> bas;
  for (unsigned int i = 1; i <= n; ++i) {
    cin >> w[i];
    w[i] -= bas;
  }
  for (unsigned int c = 0; c <= n; ++c)
    for (unsigned int v = c; v <= n; ++v)
      ans[v] = max(ans[v], ans[v - c] + w[c]);
  cout << ans[n] + (n + m) * bas << endl;
  return 0;
}
