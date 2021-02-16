#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 50000;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur),
             long long dat[])
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) {
      new (&info) val_type(dat[l]);
      return;
    }
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur, dat);
    rc->build(mv, r, cur, dat);
    pushup();
  }
  long long query(const unsigned int l, const unsigned int r) const
  {
    return queryImpl(l, r).seq;
  }
  void assign(const unsigned int p, const long long val)
  {
    if (rv - lv == 1) {
      new (&info) val_type(val);
      return;
    }
    if (p < mv)
      lc->assign(p, val);
    else
      rc->assign(p, val);
    pushup();
  }

 private:
  struct val_type {
    val_type() = default;
    val_type(long long v) : sum(v), pre(v), post(v), seq(v) {}
    val_type(long long sum, long long pre, long long post, long long seq)
        : sum(sum), pre(pre), post(post), seq(seq)
    {
    }
    long long sum, pre, post, seq;
    val_type merge(const val_type& r) const
    {
      return val_type(sum + r.sum, max(pre, sum + r.pre),
                      max(post + r.sum, r.post),
                      max({seq, r.seq, post + r.pre}));
    }
  } info;
  void pushup() { this->info = lc->info.merge(rc->info); }
  val_type queryImpl(const unsigned int l, const unsigned int r) const
  {
    if (l == lv && r == rv) return info;
    if (r <= mv)
      return lc->queryImpl(l, r);
    else if (l >= mv)
      return rc->queryImpl(l, r);
    else
      return lc->queryImpl(l, mv).merge(rc->queryImpl(mv, r));
  }

  unsigned int lv, mv, rv;

  STree *lc, *rc;
} SNodes[maxn * 4], *SRoot = SNodes;
long long a[maxn + 1];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  copy_n(istream_iterator<long long>(cin), n, a + 1);
  {
    STree* c = SNodes;
    SRoot->build(1, n + 1, c, a);
  }
  cin >> q;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int op, x;
    cin >> op >> x;
    if (op) {
      unsigned int y;
      cin >> y;
      cout << SRoot->query(x, y + 1) << endl;
    }
    else {
      long long v;
      cin >> v;
      SRoot->assign(x, v);
    }
  }
  return 0;
}