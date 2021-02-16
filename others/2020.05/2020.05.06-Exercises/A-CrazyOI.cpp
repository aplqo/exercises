/*Luogu team T132648: Crazy OI*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#ifdef LogGCD
#include <fstream>
using std::ofstream;
ofstream glog;
#define writeGcdLog(c, a, m, x) \
  glog << (c) << " " << (a) << " " << (m) << " " << (x) << endl;
#define openGcdLog(argc, argv) \
  if (argc > 0) glog.open(argv[1]);
#else
#define writeGcdLog(c, a, m, x)
#define openGcdLog(argc, argv)
#endif
using namespace std;
using num = unsigned int;
using gcd_t = int;
const int maxm = 1e6, maxn = 15;

num c[maxn + 10], p[maxn + 10], l[maxn + 10];
unsigned int n;

gcd_t exgcd(gcd_t a, gcd_t b, gcd_t& x, gcd_t& y)
{
  if (b == 0) {
    x = 1;
    y = 0;
    return a;
  }
  const gcd_t g = exgcd(b, a % b, x, y);
  gcd_t t = x;
  x = y;
  y = t - (a / b) * y;
  return g;
}
num solve(const unsigned int i, const unsigned int j, const gcd_t m)
{
  const gcd_t a = (gcd_t)p[j] - (gcd_t)p[i], cc = (gcd_t)c[i] - (gcd_t)c[j];
  gcd_t x, y;
  gcd_t g = a > m ? exgcd(a, m, x, y) : exgcd(m, a, y, x);
  if (g < 0) g = -g;
  if (cc % g) return maxm + 10;
  x *= cc / g;
  if (x < 0) x += ((-x + m / g) / (m / g)) * (m / g);
  x %= m / g;
  writeGcdLog(cc, a, m, x);
  return x;
}
bool test(const num m)
{
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = i + 1; j < n; ++j) {
      const unsigned int p = solve(i, j, m);
      if (p <= l[i] && p <= l[j]) return false;
    }
  return true;
}
int main(int argc, char* argv[])
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  openGcdLog(argc, argv);
  num lv = 0, rv = maxm;
  cin >> n;
  for (unsigned int i = 0; i < n; ++i) {
    cin >> c[i] >> p[i] >> l[i];
    --c[i];
  }
  lv = *max_element(c, c + n);
  ++lv;
  for (; lv <= maxm; ++lv)
    if (test(lv)) break;
  cout << lv << endl;
  return 0;
}