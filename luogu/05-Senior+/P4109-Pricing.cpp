#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxl = 9;

unsigned int po[maxl + 10];

static void getPow(const unsigned int n)
{
  po[0] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    po[i] = po[i - 1] * 10;
}
unsigned int toDigit(unsigned int x, unsigned int res[])
{
  unsigned int ret = 0;
  do {
    res[ret++] = x % 10;
    x /= 10;
  } while (x);
  return ret;
}
unsigned int cost(const unsigned int x)
{
  unsigned int d[maxl + 1];
  const unsigned int l = toDigit(x, d);
  for (unsigned int i = 0; i < l; ++i)
    if (d[i]) return 2 * (l - i) - (d[i] == 5);
  return 0;
}
unsigned int greedy(const unsigned int l, const unsigned int r)
{
  unsigned int dl[maxl + 10] = {}, dr[maxl + 10] = {};
  const unsigned int lg = max(toDigit(l, dl), toDigit(r, dr));
  pair<unsigned int, unsigned int> ret(UINT_MAX, 0);
  for (unsigned int i = lg; i; --i) {
    unsigned int v = 0;
    for (unsigned int j = i; j; --j) {
      const unsigned int t = v + po[j - 1] * 5;
      if (l <= t && t <= r) {
        v = t;
        goto End;
      }
      for (unsigned int d = 1; d < 10; ++d) {
        const unsigned int nv = v + po[j - 1] * d;
        if (l <= nv && nv <= r) {
          v = nv;
          goto End;
        }
      }
      v += dl[j - 1] * po[j - 1];
    }
  End:;
    if (l <= v && v <= r) ret = min(ret, make_pair(cost(v), v));
  }
  return ret.second;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  getPow(maxl);
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int l, r;
    cin >> l >> r;
    cout << greedy(l, r) << "\n";
  }
  return 0;
}