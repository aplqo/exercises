#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const int maxn = 1e8;

unsigned int primes[maxn + 10], mindiv[maxn + 10], *pc = primes;

inline void euler(unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pc++) = i;
    }
    for (unsigned int* j = primes; j < pc && *j <= mindiv[i] && *j * i <= n;
         ++j)
      mindiv[*j * i] = *j;
  }
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  unsigned int n, q;
  cin >> n >> q;
  euler(n);
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int k;
    cin >> k;
    cout << primes[--k] << endl;
  }
  return 0;
}