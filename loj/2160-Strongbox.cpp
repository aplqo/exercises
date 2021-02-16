#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 250000;

unsigned long long a[maxn + 10], fac[maxn + 10];
bool invalid[maxn + 10];

unsigned int factor(const unsigned long long v)
{
  unsigned long long* ptr = fac;
  for (unsigned long long i = 1; i * i <= v; ++i) {
    if (v % i) continue;
    *(ptr++) = i;
    if (i * i != v) *(ptr++) = v / i;
  }
  return ptr - fac;
}
void markInvalid(const unsigned int ptr, const unsigned int k,
                 const unsigned long long cand)
{
  sort(fac, fac + ptr);
  for (const unsigned long long* i = a; i < a + k - 1; ++i)
    invalid[lower_bound(fac, fac + ptr, gcd(cand, *i)) - fac] = true;
}
unsigned long long count(const unsigned long long n, const unsigned int k)
{
  const unsigned long long cand = gcd(n, a[k - 1]), ptr = factor(cand);
  markInvalid(ptr, k, cand);
  for (const unsigned long long* i = fac; i < fac + ptr; ++i) {
    if (invalid[i - fac]) continue;
    for (const unsigned long long* j = i + 1; j < fac + ptr; ++j)
      if (*j % *i == 0 && invalid[j - fac]) goto fail;
    return n / *i;
  fail:;
  }
  return 0;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int k;
  unsigned long long n;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned long long>(cin), k, a);
  cout << count(n, k) << "\n";
  return 0;
}