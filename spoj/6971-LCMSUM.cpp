#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6;

unsigned long long g[maxn + 10];

static void eular(const unsigned int n)
{
  static unsigned int mindiv[maxn + 10], primes[maxn + 10], *pcur = primes;
  g[1] = 1;
  for (unsigned long long i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
      g[i] = i * (i - 1) + 1;
    }
    for (const unsigned int* j = primes;
         j < pcur && *j < mindiv[i] && *j * i <= n; ++j) {
      const unsigned int v = *j * i;
      mindiv[v] = *j;
      g[v] = g[i] * g[*j];
    }
    if (i * mindiv[i] <= n) {
      const unsigned int v = i * mindiv[i];
      mindiv[v] = mindiv[i];
      g[v] = g[i] + (g[i] - g[i / mindiv[i]]) * mindiv[i] * mindiv[i];
    }
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  eular(maxn);
  for (unsigned int i = t; i; --i) {
    unsigned int n;
    cin >> n;
    cout << (g[n] + 1) * n / 2 << "\n";
  }
  return 0;
}