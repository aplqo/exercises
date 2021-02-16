#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6;
constexpr unsigned long long mod = 998244353;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                    \
  friend inline Number operator x(const Number l, const Number r); \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
  decl(/);
#undef decl
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return (l.val + mod) x r.val;                          \
  }
impl(+);
impl(-);
impl(*);
#undef impl
Number quickPow(Number a, unsigned long long e)
{
  Number ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number operator/(const Number l, const Number r)
{
  return l * quickPow(r, mod - 2);
}
#define lowbit(x) ((x) & -(x))
template <class T>
class FTree {
 public:
  void add(unsigned int pos, const unsigned int n, const T val)
  {
    for (; pos <= n; pos += lowbit(pos))
      this->val[pos] += val;
  }
  T sum(const unsigned int l, const unsigned int r) const
  {
    return query(r) - query(l - 1);
  }

 private:
  T query(unsigned int p) const
  {
    T ret = 0;
    for (; p; p -= lowbit(p))
      ret += val[p];
    return ret;
  }

  T val[maxn + 19] = {};
};
#undef decl

unsigned long long a[maxn + 10];

Number count(const unsigned int n, const unsigned int k)
{
  static FTree<Number> lSum;
  static FTree<int> gCnt;
  static unsigned int seq[maxn + 10];
  iota(seq, seq + n, 1);
  sort(seq, seq + n,
       [](const unsigned int l, const unsigned int r) { return a[l] < a[r]; });
  Number ret = accumulate(a + 1, a + 1 + n, Number(0));
  for (unsigned int i = 1; i <= n; ++i)
    gCnt.add(i, n, 1);
  for (unsigned int* i = seq; i < seq + n; ++i) {
    gCnt.add(*i, n, -1);
    lSum.add(*i, n, a[*i]);
    const unsigned int lp = max(int(*i) - int(k), 1);
    if (lp == *i) continue;
    ret -= (gCnt.sum(lp, *i - 1) * a[*i] + lSum.sum(lp, *i - 1)) / (*i - lp);
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned long long>(cin), n, a + 1);
  cout << count(n, k) << "\n";
  return 0;
}
