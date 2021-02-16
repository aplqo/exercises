#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 20000;

unsigned int l[maxn];

unsigned int solve(const unsigned int n, const unsigned int s)
{
  unsigned int ret = 0;
  unsigned int *pl = l, *pr = l + n - 1;
  for (; pl < pr; ++pl) {
    while (*pl + *pr > s && pr > pl)
      --pr;
    ret += pr - pl;
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, s;
  cin >> n >> s;
  for_each(l, l + n, [](unsigned int& i) -> void { cin >> i; });
  sort(l, l + n);
  cout << solve(n, s) << endl;
  return 0;
}