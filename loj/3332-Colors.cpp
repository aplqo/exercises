#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxl = 64;

inline bool setColor(const unsigned long long c)
{
  cout << "? " << c << endl;
  unsigned int ret;
  cin >> ret;
  return ret;
}
inline void giveAnswer(const unsigned long long c)
{
  cout << "= " << c << endl;
}

long long findStart(const long long n)
{
  static long long buf[maxl + 10];
  long long len = 1, *ptr = buf;
  while (len < n) {
    const long long mid = len + (n + 1 - len) / 2 - 1;
    *(ptr++) = mid;
    len = mid + 1;
  }
  long long dir = -1, pos = n;
  for (const long long* i = ptr - 1; i >= buf; --i, dir *= -1)
    pos += (*i) * dir;
  return dir == 1 ? pos : n + 1 - pos;
}

unsigned long long solve(long long n)
{
  long long acc = 0, pos = findStart(n), dir = 1;
  setColor(pos);
  while (n > 1) {
    const long long mid = n / 2;
    pos += dir * (acc + mid);
    if (setColor(pos))
      n = mid;
    else {
      acc += mid;
      n -= mid;
    }
    dir *= -1;
  }
  return acc + 1;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned long long n;
    cin >> n;
    giveAnswer(solve(n));
  }
  return 0;
}