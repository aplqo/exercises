#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 17;
constexpr unsigned long long mod = 998244353, inv2 = 499122177;

unsigned int rev[1u << maxn];

static void initRev(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    rev[1u << i] = 1u << (n - 1 - i);
  const unsigned int msk = 1u << n;
  for (unsigned int i = 3; i < msk; ++i) {
    const unsigned int lb = i & -i;
    rev[i] = rev[i ^ lb] | rev[lb];
  }
}
inline void reverseCopy(unsigned long long dest[],
                        const unsigned long long src[], const unsigned int sz)
{
  for (unsigned int i = 0; i < sz; ++i)
    dest[rev[i]] = src[i];
}
template <class Func>
void fwt(const unsigned long long src[], unsigned long long dest[],
         const unsigned int n, Func pred)
{
  reverseCopy(dest, src, n);
  for (unsigned int step = 2; step <= n; step <<= 1) {
    const unsigned int half = step >> 1;
    for (unsigned int blk = 0; blk < n; blk += step)
      for (unsigned int i = 0; i < half; ++i)
        pred(dest[blk + i], dest[blk + i + half]);
  }
}
template <class Func, class iFunc>
void fwt(const unsigned int n, const unsigned long long a[],
         const unsigned long long b[], unsigned long long res[], Func pred,
         iFunc ipred)
{
  static unsigned long long tmpa[1u << maxn], tmpb[1u << maxn];
  const unsigned int sz = 1u << n;
  fwt(a, tmpa, sz, pred);
  fwt(b, tmpb, sz, pred);
  for (unsigned int i = 0; i < sz; ++i)
    tmpb[i] = (tmpb[i] * tmpa[i]) % mod;
  fwt(tmpb, res, sz, ipred);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  initRev(n);
  const unsigned int sz = 1u << n;
  static unsigned long long a[1u << maxn], b[1u << maxn], res[1u << maxn];
  copy_n(istream_iterator<unsigned int>(cin), sz, a);
  copy_n(istream_iterator<unsigned int>(cin), sz, b);

  // or
  fwt(
      n, a, b, res,
      [](unsigned long long a, unsigned long long& b) { b = (b + a) % mod; },
      [](unsigned long long a, unsigned long long& b) {
        b = (b + mod - a) % mod;
      });
  copy(res, res + sz, ostream_iterator<unsigned long long>(cout, " "));
  cout.put('\n');

  // and
  fwt(
      n, a, b, res,
      [](unsigned long long& a, unsigned long long b) { a = (a + b) % mod; },
      [](unsigned long long& a, unsigned long long b) {
        a = (a + mod - b) % mod;
      });
  copy(res, res + sz, ostream_iterator<unsigned long long>(cout, " "));
  cout.put('\n');

  // xor
  fwt(
      n, a, b, res,
      [](unsigned long long& a, unsigned long long& b) {
        const unsigned long long ta = a, tb = b;
        a = (ta + tb) % mod;
        b = (ta + mod - tb) % mod;
      },
      [](unsigned long long& a, unsigned long long& b) {
        const unsigned long long ta = a, tb = b;
        a = ((ta + tb) % mod * inv2) % mod;
        b = ((ta + mod - tb) % mod * inv2) % mod;
      });
  copy(res, res + sz, ostream_iterator<unsigned long long>(cout, " "));
  cout.put('\n');
  return 0;
}