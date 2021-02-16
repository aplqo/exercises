#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const int maxn = 40000;

unsigned int phi[maxn + 10] = {0, 1};
unsigned int mindiv[maxn + 10], primes[maxn + 10], *pc = primes;

inline void euler(unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      phi[i] = i - 1;
      *(pc++) = i;
    }
    for (unsigned int* p = primes; p < pc && *p <= mindiv[i] && *p * i <= n;
         ++p) {
      mindiv[*p * i] = *p;
      phi[*p * i] = *p != mindiv[i] ? phi[i] * (*p - 1) : phi[i] * (*p);
    }
  }
}
int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, ans = 0;
  cin >> n;
  if (n == 1) {
    cout << "0" << endl;
    return 0;
  }
  euler(n);
  for (unsigned int i = 1; i < n; ++i)
    ans += phi[i];
  ans = (ans * 2) + 1;
  cout << ans << endl;
  return 0;
}