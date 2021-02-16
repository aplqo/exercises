#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;

unsigned int highbit(unsigned int x)
{
  for (unsigned int i = 1u << 31; i; i >>= 1)
    if (x & i) return i;
  return 0;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int n, s;
    cin >> n >> s;
    if (s)
      cout << (n + 1) / 2 << "\n";
    else {
      if (!(n & 0x01)) n -= highbit(n);
      cout << n / 2 << "\n";
    }
  }
  return 0;
}