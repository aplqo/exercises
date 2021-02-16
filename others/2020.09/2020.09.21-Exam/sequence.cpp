#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1000000, maxl = 31;

unsigned int a[maxn + 10];
int cnt[maxl + 10];

inline void change(unsigned int x, const int v)
{
  for (unsigned int i = 0, msk = 1; i < maxl; ++i, msk <<= 1)
    if (x & msk) cnt[i] += v;
}
inline unsigned int toUint(const unsigned int k)
{
  unsigned int ret = 0;
  for (unsigned int i = 0, msk = 1; i < maxl; ++i, msk <<= 1)
    if (cnt[i] == k) ret |= msk;
  return ret;
}
unsigned int maxAndSum(const unsigned int n, const unsigned int k)
{
  unsigned int ret = 0;
  for (const unsigned int* i = a; i < a + k; ++i)
    change(*i, 1);
  ret = toUint(k);
  for (const unsigned int* i = a + k; i < a + n; ++i) {
    change(*(i - k), -1);
    change(*i, 1);
    ret = max(ret, toUint(k));
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  cout << accumulate(a, a + n, 0u, bit_or<unsigned int>()) << " "
       << maxAndSum(n, k) << "\n";
  return 0;
}