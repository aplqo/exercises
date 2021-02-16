#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e7;

unsigned long long f[maxn + 10] = {0, 1};
unsigned int mindiv[maxn + 10], primes[maxn + 10], *pcur = primes;

void eular(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
      f[i] = i - 2;
    }
    for (unsigned int* j = primes; j < pcur && *j < mindiv[i] && *j * i <= n;
         ++j) {
      mindiv[*j * i] = *j;
      f[*j * i] = f[i] * (*j - 2);
    }
    const unsigned long long t = 1ull * i * mindiv[i];
    if (t <= n) {
      mindiv[t] = mindiv[i];
      f[t] = f[i] * mindiv[i];
    }
  }
}
int main()
{
#ifdef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  eular(maxn);
  for (unsigned int i = 0; i < t; ++i) {
    unsigned int n;
    cin >> n;
    cout << f[n] << endl;
  }
  return 0;
}