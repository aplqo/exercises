#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
constexpr unsigned int primes[4] = {2, 3, 4679, 35617}, mod = 999911659,
                       phi = mod - 1;

unsigned long long fac[primes[3] + 10], ifac[primes[3] + 10];

template <unsigned long long mod>
constexpr unsigned int quickPow(unsigned long long x, unsigned int e)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * x) % mod;
    x = (x * x) % mod;
  }
  return ret;
}
template <unsigned long long mod>
constexpr inline unsigned int inverse(unsigned int x)
{
  return quickPow<mod>(x, mod - 2);
}
template <unsigned long long mod>
constexpr unsigned int getFactor()
{
  unsigned long long v = 1;
  for (unsigned int i = 0; i < 4; ++i)
    if (primes[i] != mod) v *= primes[i];
  return (inverse<mod>(v) * v) % phi;
}

template <unsigned long long mod>
void initModularInv()
{
  static unsigned long long prd[mod + 10], iprd[mod + 10];
  prd[0] = 1;
  iprd[0] = 1;
  for (unsigned int i = 1; i < mod; ++i)
    prd[i] = (prd[i - 1] * fac[i]) % mod;
  iprd[mod - 1] = inverse<mod>(prd[mod - 1]);
  for (unsigned int i = mod - 1; i; --i) {
    iprd[i - 1] = (iprd[i] * fac[i]) % mod;
    ifac[i] = (iprd[i] * prd[i - 1]) % mod;
  }
  ifac[0] = 1;
}
template <unsigned long long mod>
void initFactor()
{
  fac[0] = ifac[0] = 1;
  for (unsigned int i = 1; i < mod; ++i)
    fac[i] = (fac[i - 1] * i) % mod;
}
template <unsigned long long mod>
inline unsigned long long c(unsigned int n, unsigned int m)
{
  static const auto fun = [](unsigned long long n, unsigned long long m) {
    return n >= m ? ((fac[n] * ifac[n - m]) % mod * ifac[m]) % mod : 0;
  };
  unsigned long long ret = 1;
  while (n && m) {
    ret = (ret * fun(n % mod, m % mod)) % mod;
    n /= mod;
    m /= mod;
  }
  return ret;
}
template <unsigned long long mod>
unsigned long long sumOne(const unsigned int n)
{
  unsigned long long ret = 0;
  initFactor<mod>();
  initModularInv<mod>();
  for (unsigned int i = 1; i * i <= n; ++i) {
    if (n % i) continue;
    ret += c<mod>(n, i);
    if (n / i != i) ret += c<mod>(n, n / i);
    ret %= mod;
  }
  return ret;
}
unsigned int solve(const unsigned int n)
{
  return ((sumOne<primes[0]>(n) * getFactor<primes[0]>()) % phi +
          (sumOne<primes[1]>(n) * getFactor<primes[1]>()) % phi +
          (sumOne<primes[2]>(n) * getFactor<primes[2]>()) % phi +
          (sumOne<primes[3]>(n) * getFactor<primes[3]>()) % phi) %
         phi;
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, g;
  std::cin >> n >> g;
  if (g % mod == 0)
    std::cout << "0\n";
  else
    std::cout << quickPow<mod>(g, solve(n)) << "\n";
  return 0;
}