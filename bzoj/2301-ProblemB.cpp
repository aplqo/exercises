#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxa = 5e4;

long long sum[maxa + 10];

static void eular(const unsigned int n)
{
  static unsigned int mindiv[maxa + 10], primes[maxa + 10], *pcur = primes;
  static int mu[maxa + 10];
  mu[1] = 1;
  for (unsigned long long i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
      mu[i] = -1;
    }
    for (const unsigned int* j = primes;
         j < pcur && *j < mindiv[i] && *j * i <= n; ++j) {
      mindiv[*j * i] = *j;
      mu[*j * i] = -mu[i];
    }
    if (i * mindiv[i] <= n) mindiv[i * mindiv[i]] = mindiv[i];
  }
  for (unsigned int i = 1; i <= n; ++i)
    sum[i] = sum[i - 1] + mu[i];
}
long long count(unsigned int a, unsigned int b, const unsigned int k)
{
  unsigned long long ret = 0;
  a /= k;
  b /= k;
  for (unsigned int i = 1; i <= a && i <= b;) {
    const unsigned int va = a / i, vb = b / i;
    const unsigned int nxt = min(a / va, b / vb);
    ret += static_cast<long long>(va) * vb * (sum[nxt] - sum[i - 1]);
    i = nxt + 1;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  eular(maxa);
  cin >> t;
  for (unsigned int i = t; i; --i) {
    unsigned int a, b, c, d, k;
    cin >> a >> b >> c >> d >> k;
    cout << count(b, d, k) + count(a - 1, c - 1, k) - count(a - 1, d, k) -
                count(b, c - 1, k)
         << "\n";
  }
  return 0;
}