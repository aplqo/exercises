#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxq = 1e5, maxl = 20;
constexpr unsigned long long mod = 1e9 + 7;

class LinearBasis {
 public:
  void insert(unsigned int x);
  bool query(unsigned int x) const;
  unsigned int size() const;

 private:
  unsigned int val[maxl + 1] = {};
};
struct Query {
  unsigned int l, x;
  unsigned long long ans;
} qry[maxq + 1];
Query* seq[maxq + 1];
unsigned int a[maxn + 1];

void LinearBasis::insert(unsigned int x)
{
  for (int i = maxl; i >= 0 && x; --i) {
    if (!(x & (1 << i))) continue;
    if (val[i])
      x ^= val[i];
    else {
      for (unsigned int j = 0; j < i; ++j)
        if (x & (1 << j)) x ^= val[j];
      for (unsigned int j = i + 1; j <= maxl; ++j)
        if (val[j] & (1 << i)) val[j] ^= x;
      val[i] = x;
      break;
    }
  }
}
bool LinearBasis::query(unsigned int x) const
{
  for (int i = maxl; i >= 0 && x; --i) {
    if (!(x & (1 << i))) continue;
    if (val[i])
      x ^= val[i];
    else {
      for (unsigned int j = 0; j < i; ++j)
        if (x & (1 << j)) x ^= val[j];
      return !x;
    }
  }
  return true;
}
unsigned int LinearBasis::size() const
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i <= maxl; ++i)
    if (val[i]) ++ret;
  return ret;
}

unsigned long long quickPow(unsigned long long a, unsigned long long e)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * a) % mod;
    a = (a * a) % mod;
  }
  return ret;
}
static void solveQuery(const unsigned int n, const unsigned int q)
{
  transform(qry, qry + q, seq, [](Query& i) { return &i; });
  sort(seq, seq + q, [](Query* a, Query* b) { return a->l < b->l; });
  LinearBasis lb;
  unsigned int ptr = 0;
  for (unsigned int i = 0; i < q; ++i) {
    Query& cq = *seq[i];
    while (ptr <= n && cq.l > ptr)
      lb.insert(a[++ptr]);
    cq.ans = lb.query(cq.x) ? quickPow(2, cq.l - lb.size()) : 0;
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  for (Query* i = qry; i < qry + q; ++i)
    cin >> i->l >> i->x;
  solveQuery(n, q);
  for (const Query* i = qry; i < qry + q; ++i)
    cout << i->ans << "\n";
  return 0;
}