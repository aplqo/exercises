#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxq = 40000, maxd = 1000000;

unsigned int mindiv[maxq + 10], primes[maxq + 10], *pcur = primes;
bool notPrime[maxd + 10];

void eular(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
    }
    for (unsigned int* j = primes; j < pcur && *j * i <= n && *j <= mindiv[i];
         ++j)
      mindiv[*j * i] = *j;
  }
}
void eratosh(const unsigned long long l, const unsigned long long r)
{
  const static auto squ = [](const unsigned int i) -> unsigned long long {
    return i * i;
  };
  for (unsigned int* i = primes; i < pcur && squ(*i) <= r; ++i) {
    const unsigned long long pr = *i;
    for (unsigned long long j = (l + pr - 1) / pr * pr; j <= r; j += pr)
      notPrime[j - l] = true;
    if (pr >= l) notPrime[pr - l] = false;
  }
}
void solve(const unsigned long long l, const unsigned long long r)
{
  fill(notPrime, notPrime + r - l + 1, false);
  if (l == 1) notPrime[0] = true;
  eratosh(l, r);
  for (unsigned long long i = 0; i < r - l + 1; ++i)
    if (!notPrime[i]) cout << i + l << endl;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  eular(maxq);
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int m, n;
    cin >> m >> n;
    solve(m, n);
    if (i != t - 1) cout << endl;
  }
  return 0;
}
