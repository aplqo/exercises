#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <numeric>
const unsigned int maxk = 10;

int a[maxk + 10], b[maxk + 10];

unsigned long long multiply(unsigned long long a, unsigned long long b,
                            const unsigned long long mod)
{
  unsigned long long ret = 0;
  for (; b; b >>= 1) {
    if (b & 0x01) ret = (ret + a) % mod;
    a = (a + a) % mod;
  }
  return ret;
}
unsigned long long quickPow(unsigned long long a, unsigned long long e,
                            const unsigned long long mod)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = multiply(ret, a, mod);
    a = multiply(a, a, mod);
  }
  return ret;
}
unsigned int phi(const unsigned int x)
{
  unsigned int ret = x, v = x;
  for (unsigned int i = 2; i * i <= v && v > 1; ++i) {
    if (v % i) continue;
    ret = ret / i * (i - 1);
    while (!(v % i))
      v /= i;
  }
  if (v > 1) ret = ret / v * (v - 1);
  return ret;
}
inline unsigned long long inverse(const unsigned long long a,
                                  const unsigned long long e)
{
  return e >= 2 ? quickPow(a, phi(e) - 1, e) : 1;
}

unsigned long long crt(const unsigned int k)
{
  unsigned long long ret = 0;
  const unsigned long long prd =
      std::accumulate(b, b + k, 1ull, std::multiplies<unsigned long long>());
  for (unsigned int i = 0; i < k; ++i) {
    const unsigned long long v =
        multiply(inverse(prd / b[i], b[i]), prd / b[i], prd);
    ret = (ret + multiply((a[i] % b[i] + b[i]) % b[i], v, prd)) % prd;
  }
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int k;
  std::cin >> k;
  std::copy_n(std::istream_iterator<int>(std::cin), k, a);
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), k, b);
  std::cout << crt(k) << "\n";
  return 0;
}