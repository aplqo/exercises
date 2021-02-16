#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 100000;

int beg[maxn + 2], ed[maxn + 2];

unsigned long long backDist(const unsigned int n, const unsigned int m)
{
  beg[n] = m;
  ed[n] = 0;
  sort(beg, beg + n + 1);
  sort(ed, ed + n + 1);
  unsigned long long ret = 0;
  for (unsigned int i = 0; i <= n; ++i)
    ret += abs(beg[i] - ed[i]);
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  long long ans = 0;
  cin >> n >> m;
  for (unsigned int i = 0; i < n; ++i) {
    cin >> beg[i] >> ed[i];
    ans += abs(beg[i] - ed[i]);
  }
  cout << ans + backDist(n, m) << "\n";
  return 0;
}