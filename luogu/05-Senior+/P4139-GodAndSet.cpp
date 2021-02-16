#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num_t = unsigned long long;

inline num_t phi(num_t p)
{
  num_t ret = p, orig = p;
  for (num_t i = 2; i * i <= orig; ++i)
    if (!(p % i)) {
      ret = ret / i * (i - 1);
      for (; !(p % i); p /= i)
        ;
    }
  if (p > 1) ret = ret / p * (p - 1);
  return ret;
}
inline num_t QuickPow(num_t a, num_t ex, const num_t mod)
{
  num_t ret = 1;
  a %= mod;
  for (num_t i = 1; ex; i <<= 1) {
    if (ex & i) {
      ret = (ret * a) % mod;
      ex ^= i;
    }
    a = (a * a) % mod;
  }
  return ret;
}
num_t solve(num_t v)
{
  if (v == 1) return 0;
  const num_t p = phi(v);
  return v & 0x01u ? QuickPow(2, solve(p), v) : QuickPow(2, solve(p) + p, v);
}

int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    num_t p;
    cin >> p;
    cout << solve(p) << endl;
  }
  return 0;
}