#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <utility>
using namespace std;
const unsigned int maxn = 1e5;
using Equation = pair<long long, long long>;  // x = first ( mod second )

long long a[maxn + 1], p[maxn + 1], t[maxn + 1], ini[maxn + 1], mnx[maxn + 1];
Equation at[maxn + 1];

long long multiply(long long a, long long b, const long long mod) noexcept
{
  long long ret = 0;
  for (; b; b >>= 1) {
    if (b & 0x01) ret = (ret + a) % mod;
    a = (a + a) % mod;
  }
  return ret;
}
long long exgcd(const long long a, const long long b, long long& x,
                long long& y) noexcept
{
  if (!b) {
    x = 1;
    y = 0;
    return a;
  }
  const long long g = exgcd(b, a % b, x, y);
  const long long t = x;
  x = y;
  y = t - (a / b) * y;
  return g;
}
long long toPositive(long long x, const long long m) noexcept
{
  x %= m;
  if (x < 0) x += m;
  return x;
}
Equation solveEqu(const long long a, const long long b,
                  const long long c)  // Solve ax + by = c returns x
{
  long long x, y;
  const long long g = a > b ? exgcd(a, b, x, y) : exgcd(b, a, y, x);
  if (c % g) throw 0;
  const long long m = b / g;
  return Equation(multiply(toPositive(x, m), c / g, m), m);
}
inline Equation solveEquGroup(Equation a, Equation b)
{
  if (a.first > b.first) swap(a, b);
  const long long t = solveEqu(a.second, b.second, b.first - a.first).first;
  const long long lc = lcm(a.second, b.second);
  return Equation(((t * a.second) % lc + a.first) % lc, lc);
}
void select(const unsigned int n, const unsigned int m)
{
  multiset<long long> kn;
  kn.insert(ini, ini + m);
  for (unsigned int i = 0; i < n; ++i) {
    auto it = kn.upper_bound(a[i]);
    if (it != kn.begin()) --it;
    at[i] = solveEqu(*it, p[i], (a[i]) % p[i]);
    mnx[i] = (a[i] + *it - 1) / *it;
    kn.erase(it);
    kn.insert(t[i]);
  }
}
long long solveMultEqu(const unsigned int n)
{
  Equation lst = at[0];
  for (unsigned int i = 1; i < n; ++i)
    lst = solveEquGroup(lst, at[i]);
  return max(lst.first ? lst.first : lst.second, *max_element(mnx, mnx + n));
}
void solve(istream& ins, ostream& outs)
{
  unsigned int n, m;
  ins >> n >> m;
  copy_n(istream_iterator<long long>(ins), n, a);
  copy_n(istream_iterator<long long>(ins), n, p);
  copy_n(istream_iterator<long long>(ins), n, t);
  copy_n(istream_iterator<long long>(ins), m, ini);
  select(n, m);
  outs << solveMultEqu(n) << endl;
}
int main()
{
#if !defined APTEST && !defined APDEBUG
  ios_base::sync_with_stdio(false);
  ifstream inp("dragon.in");
  ofstream outp("dragon.out");
#else
  istream& inp = cin;
  ostream& outp = cout;
#endif
  unsigned int t;
  inp >> t;
  for (unsigned int i = 0; i < t; ++i) {
    try {
      solve(inp, outp);
    }
    catch (...) {
      outp << "-1" << endl;
    }
  }
  return 0;
}