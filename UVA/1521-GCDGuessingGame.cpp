#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 10000;

unsigned int primes[maxn + 10], *pcur = primes;
unsigned int mindiv[maxn + 10];

static void eular(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      *(pcur++) = i;
      mindiv[i] = i;
    }
    for (const unsigned int* j = primes;
         j < pcur && static_cast<unsigned long long>(*j) * i <= n &&
         *j <= mindiv[i];
         ++j)
      mindiv[*j * i] = *j;
  }
}
unsigned int count(const unsigned int n)
{
  unsigned int ret = 0;
  const unsigned int* l = primes;
  for (const unsigned int* r = upper_bound(primes, pcur, n) - 1; l <= r;
       --r, ++ret) {
    unsigned long long acc = *r;
    while (acc * (*l) <= n)
      acc *= *(l++);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  eular(maxn);
  while (cin) {
    unsigned int n;
    cin >> n;
    if (cin.fail()) break;
    cout << count(n) << "\n";
  }
  return 0;
}