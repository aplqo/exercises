#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxl = 10;

unsigned int f[maxl + 2][10] = {{}, {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

static void init(const unsigned int l)
{
  for (unsigned int i = 2; i <= l; ++i)
    for (int j = 0; j < 10; ++j)
      for (int k = 0; k < 10; ++k)
        if (abs(j - k) >= 2) f[i][j] += f[i - 1][k];
}
unsigned int toDigits(unsigned int v, int digits[])
{
  unsigned int p = 0;
  do {
    digits[++p] = v % 10;
    v /= 10;
  } while (v);
  return p;
}
unsigned int dp(const unsigned int v)
{
  if (v < 10) return v;
  int digits[maxl + 2];
  unsigned int ret = 0;
  const unsigned int l = toDigits(v, digits);

  for (unsigned int i = 1; i < l; ++i)
    ret = accumulate(f[i] + 1, f[i] + 10, ret);
  ret = accumulate(f[l] + 1, f[l] + digits[l], ret);

  for (unsigned int i = l - 1; i > 0; --i) {
    for (int j = 0; j < digits[i]; ++j)
      if (abs(j - digits[i + 1]) >= 2) ret += f[i][j];
    if (abs(digits[i] - digits[i + 1]) < 2)
      break;
    else if (i == 1)
      ++ret;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int a, b;
  cin >> a >> b;
  init(ceil(log10(b)));
  cout << dp(b) - dp(a - 1) << endl;
  return 0;
}