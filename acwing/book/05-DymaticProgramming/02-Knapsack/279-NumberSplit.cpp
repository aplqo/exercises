#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdint>
#include <iostream>
using namespace std;
using num = uint_fast32_t;
const int maxn = 4000;
constexpr num msk = (num(1) << 31) - 1;

num f[maxn + 10] = {1};

num dp(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = i; j <= n; ++j)
      f[j] = (f[j] + f[j - i]) & msk;
  return f[n] - 1;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  cout << dp(n) << endl;
  return 0;
}