#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <bitset>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxa = 3e8, maxb = 3e4;

unsigned int primes[maxb + 19], *pcur = primes;

static void eular(const unsigned int b)
{
  static unsigned int mindiv[maxb + 10];
  for (unsigned int i = 2; i <= b; ++i) {
    if (!mindiv[i]) {
      *(pcur++) = i;
      mindiv[i] = i;
    }
    for (const unsigned int* j = primes;
         j < pcur && 1ull * (*j) * i <= b && *j <= mindiv[i]; ++j)
      mindiv[*j * i] = *j;
  }
}
static unsigned int sleve(const unsigned int l, const unsigned int r)
{
  static bool notPrime[maxb + 10];
  unsigned int ret = 0;
  fill(notPrime, notPrime + (r - l), false);
  for (const unsigned int* j = primes; j < pcur; ++j) {
    const unsigned int v = *j;
    for (unsigned int i = (l + v - 1) / v * v; i <= r; i += v)
      notPrime[i - l] = true;
    if (v >= l && v < r) notPrime[v - l] = false;
  }
  if (1 >= l) notPrime[1 - l] = true;
  for (unsigned int i = l; i < r; ++i)
    if (!notPrime[i - l] && i % 4 == 1) ++ret;
  return ret + (l <= 2 && 2 < r);
}
unsigned int count(const unsigned int l, const unsigned int r)
{
  unsigned int ret = 0;
  for (unsigned int i = l; i <= r; i += maxb)
    ret += sleve(i, min(r + 1, i + maxb));
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int l, r;
  cin >> l >> r;
  eular(sqrt(r) + 10);
  cout << count(l, r) << "\n";
  return 0;
}
