#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <bitset>
#include <iostream>
using namespace std;
const unsigned int maxn = 2000000;

bitset<maxn + 10> bs;

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, acc = 0;
  cin >> n;
  bs.set(0);
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int a;
    cin >> a;
    acc += a;
    bs ^= (bs << a);
  }
  unsigned int ans = 0;
  for (unsigned int i = 0; i <= acc; ++i)
    if (bs.test(i)) ans ^= i;
  cout << ans << "\n";
  return 0;
}