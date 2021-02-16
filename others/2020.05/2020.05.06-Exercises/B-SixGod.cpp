/*Luogu team T132654: six god*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
using namespace std;
using num = unsigned long long;
using res_t = long long;
const int maxn = 20;

unsigned int n;
num dis[maxn + 10], a, b;
res_t ans;

void fun(const unsigned int pos, const unsigned int dep = 0, const num lc = 6)
{
  if (pos >= n) {
    if (dep & 0x01)
      ans -= b / lc - (a - 1) / lc;
    else
      ans += b / lc - (a - 1) / lc;
    return;
  }
  fun(pos + 1, dep + 1, lcm(lc, dis[pos]));
  fun(pos + 1, dep, lc);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n;
  for (unsigned int i = 0; i < n; ++i)
    cin >> dis[i];
  cin >> a >> b;
  fun(0);
  cout << ans << endl;
  return 0;
}
