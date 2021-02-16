#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
using namespace std;
const int maxn = 1 << 11;

unsigned long long cir[maxn + 1], *ccur = cir;
unsigned long long l, r;
long long ans;

unsigned long long gcd(unsigned long long a, unsigned long long b)
{
  if (a < b) swap(a, b);
  while (b) {
    const unsigned long long t = a % b;
    a = b;
    b = t;
  }
  return a;
}
unsigned long long lcm(const unsigned long long a, const unsigned long long b)
{
  return a / gcd(a, b) * b;
}
void getCirno(const unsigned long long r, const long long cur = 0)
{
  if (cur > r) return;
  *(ccur++) = cur;
  getCirno(r, cur * 10 + 6);
  getCirno(r, cur * 10 + 8);
}
void inex(const unsigned long long* p, const unsigned int sel = 0,
          const unsigned long long cv = 1)
{
  if (cv > r) return;
  if (p >= ccur) {
    if (!sel) return;
    if (sel & 0x01)
      ans += r / cv - (l - 1) / cv;
    else
      ans -= r / cv - (l - 1) / cv;
    return;
  }
  inex(p + 1, sel + 1, lcm(*p, cv));
  inex(p + 1, sel, cv);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> l >> r;
  getCirno(r);
  sort(cir + 1, ccur, greater<unsigned long long>());
  inex(cir + 1);
  cout << ans << endl;
  return 0;
}