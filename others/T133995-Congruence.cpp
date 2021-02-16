#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;

void exgcd(const long long a, const long long b, long long& x, long long& y)
{
  if (!b) {
    x = 1;
    y = 0;
    return;
  }
  exgcd(b, a % b, x, y);
  long long t = x;
  x = y;
  y = t - (a / b) * y;
}
long long solve(const long long a, const long long b)
{
  long long x, y;
  exgcd(a, b, x, y);
  if (x < 0) x += (-x + b) / b * b;
  x %= b;
  return x;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  long long a, b;
  cin >> a >> b;
  cout << solve(a, b) << endl;
  return 0;
}