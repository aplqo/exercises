#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur),
             unsigned long long dat[])
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) {
      val = dat[l];
      one = dat[l] == 1;
      return;
    }
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur, dat);
    rc->build(mv, r, cur, dat);
    pushup();
  }
  void sqrtRange(const unsigned int l, const unsigned int r)
  {
    if (one) return;
    if (rv - lv == 1) {
      val = sqrt(val);
      one = val == 1;
      return;
    }
    if (r <= mv)
      lc->sqrtRange(l, r);
    else if (l >= mv)
      rc->sqrtRange(l, r);
    else {
      lc->sqrtRange(l, mv);
      rc->sqrtRange(mv, r);
    }
    pushup();
  }
  unsigned long long query(const unsigned int l, const unsigned int r) const
  {
    if (one) return r - l;
    if (l == lv && r == rv) return val;
    if (r <= mv)
      return lc->query(l, r);
    else if (l >= mv)
      return rc->query(l, r);
    else
      return lc->query(l, mv) + rc->query(mv, r);
  }

 private:
  void pushup()
  {
    this->val = lc->val + rc->val;
    this->one = lc->one && rc->one;
  }

  unsigned int lv, mv, rv;
  STree *lc, *rc;
  unsigned long long val;
  bool one;
} SNodes[maxn * 4 + 1], *SRoot = SNodes;
unsigned long long val[maxn + 1];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n;
  copy_n(istream_iterator<unsigned long long>(cin), n, val + 1);
  {
    STree* c = SNodes;
    SRoot->build(1, n + 1, c, val);
  }
  cin >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int k, l, r;
    cin >> k >> l >> r;
    if (l > r) swap(l, r);
    if (k == 0)
      SRoot->sqrtRange(l, r + 1);
    else
      cout << SRoot->query(l, r + 1) << endl;
  }
  return 0;
}