#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e6, maxq = 1e6, maxa = 1e6;

unsigned int a[maxn + 10], pre[maxn + 10];
struct Query {
  unsigned int l, r;
  unsigned int ret;
} qry[maxq + 10];
Query* seq[maxn + 10];

namespace FTree {
#define lowbit(x) ((x) & -(x))
int val[maxn + 10];

void add(unsigned int p, const unsigned int n, const int delt)
{
  for (; p <= n; p += lowbit(p))
    val[p] += delt;
}
unsigned int prefixSum(unsigned int p)
{
  int ret = 0;
  for (; p; p -= lowbit(p))
    ret += val[p];
  return ret;
}
inline unsigned int sum(const unsigned int l, const unsigned int r)
{
  return prefixSum(r) - prefixSum(l - 1);
}

#undef lowbit
}  // namespace FTree

static void init(const unsigned int n)
{
  static unsigned int lst[maxa + 10];
  for (unsigned int i = 1; i <= n; ++i) {
    pre[i] = lst[a[i]];
    lst[a[i]] = i;
  }
}
void solveQuery(const unsigned int n, const unsigned int q)
{
  transform(qry, qry + q, seq, [](Query& i) { return &i; });
  sort(seq, seq + q, [](Query* a, Query* b) { return a->r < b->r; });
  unsigned int ptr = 0;
  for (unsigned int i = 0; i < q; ++i) {
    for (const unsigned int r = seq[i]->r; ptr < r;) {
      ++ptr;
      if (pre[ptr]) FTree::add(pre[ptr], n, -1);
      FTree::add(ptr, n, 1);
    }
    seq[i]->ret = FTree::sum(seq[i]->l, seq[i]->r);
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  cin >> q;
  for (Query* i = qry; i < qry + q; ++i)
    cin >> i->l >> i->r;
  init(n);
  solveQuery(n, q);
  for (const Query* i = qry; i < qry + q; ++i)
    cout << i->ret << "\n";
  return 0;
}