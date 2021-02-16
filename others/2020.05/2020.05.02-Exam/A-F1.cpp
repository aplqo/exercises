/*Luogu team T132155: F1*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>

#include "debug_tools/checked.h"
using namespace std;
using Number = apdebug::checked::CheckedInteger<unsigned long long>;

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  Number n, ans = 0;
  cin >> n;
  for (Number t = 1; t <= n; t *= 2) {
    const Number v = (n - t) / (t * 2) + 1;
    ans += v * v;
  }
  cout << ans << endl;
  return 0;
}