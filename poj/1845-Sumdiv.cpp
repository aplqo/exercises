#ifdef APDEBUG
#undef _MSC_VER
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <iostream>
#include <utility>
using namespace std;
typedef unsigned long long num_t;
typedef unsigned int exp_t;
typedef pair<num_t, exp_t> pair_t;
const int maxl = 29, maxq = 7071;
const num_t mod = 9901;

num_t po[maxl + 10], ans = 1;
num_t primes[maxq + 10], mindiv[maxq + 10], *pc = primes;
num_t a;
exp_t b;
pair_t p[maxq + 10], *cur = p;

inline num_t QuickPow(exp_t e)
{
  num_t ret = 1;
  exp_t t = 1;
  for (exp_t i = 0; e; ++i, t <<= 1)
    if (e & t) {
      ret = (ret * po[i]) % mod;
      e ^= t;
    }
  return ret;
}
inline void getPow(num_t p, exp_t val)
{
  po[0] = p;
  for (exp_t i = 1; i <= val; ++i)
    po[i] = (po[i - 1] * po[i - 1]) % mod;
}
num_t sum(num_t p, exp_t e)
{
  if (e == 0) return 0;
  if (e == 1) return p % mod;
  num_t tmp = sum(p, e >> 1);
  if (e & 0x01)
    return (tmp + (tmp * QuickPow(e >> 1)) % mod + QuickPow(e)) % mod;
  else
    return (tmp + (tmp * QuickPow(e >> 1)) % mod) % mod;
}
inline void euler(num_t en)
{
  for (num_t i = 2; i <= en; ++i) {
    if (!mindiv[i]) {
      *(pc++) = i;
      mindiv[i] = i;
    }
    for (num_t* p = primes; p < pc && *p <= mindiv[i] && (*p) * i <= en; ++p)
      mindiv[*p * i] = *p;
  }
}
void getDiv(num_t a)
{
  for (num_t* i = primes; a > 1 && i < pc; ++i)
    if (!(a % *i)) {
      cur->first = *i;
      while (!(a % *i)) {
        a /= *i;
        ++(cur->second);
      }
      ++cur;
    }
  if (a > 1) *(cur++) = make_pair(a, exp_t(1));
}

int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cin >> a >> b;
  if (b == 0 || a == 0) {
    cout << "1" << endl;
    return 0;
  }
  euler(sqrt(a) + 1);
  getDiv(a);
  for (pair_t* i = p; i < cur; ++i)
    i->second *= b;
  for (pair_t* i = p; i < cur; ++i) {
    getPow(i->first, log2(i->second) + 1);
    ans = (ans * (sum(i->first, i->second) + 1)) % mod;
  }
  cout << ans << endl;
  return 0;
}