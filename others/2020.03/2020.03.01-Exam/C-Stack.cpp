#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <utility>
using namespace std;
using kyon = pair<int, int>;  // first:w second:s
const int maxn = 100000;

kyon q[maxn + 10];

bool cmp(const kyon a, const kyon b)
{
  return max(-a.second, a.first - b.second) <
         max(-b.second, b.first - a.second);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  int ans = INT_MIN;
  cin >> n;
  for (kyon* i = q; i < q + n; ++i)
    cin >> i->first >> i->second;
  sort(q, q + n, cmp);
  {
    int acc = 0;
    for (kyon* i = q; i < q + n; ++i) {
      ans = max(ans, acc - i->second);
      acc += i->first;
    }
  }
  cout << ans << endl;
  return 0;
}
