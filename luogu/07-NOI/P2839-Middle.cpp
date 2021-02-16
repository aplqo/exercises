#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 20000;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, const unsigned int p)
  {
    if (r - l == 1) {
      val = l == p ? -1 : 1;
      return;
    }
    STree* c = create(l, mv);
    c->build(l, mv, p);
    lc = c;
    c = create(mv, r);
    c->build(mv, r, p);
    rc = c;
    this->val = lc->val + rc->val;
  }
  inline int query(const unsigned int l1, const unsigned int l2,
                   const unsigned int r1, const unsigned int r2) const
  {
    return queryImpl(l1, l2 + 1).post +
           (l2 + 1 != r1 ? queryImpl(l2 + 1, r1).sum : 0) +
           queryImpl(r1, r2 + 1).pre;
  }
  void negate(const STree* old, const unsigned int p)
  {
    if (rv - lv == 1) {
      val = -1;
      return;
    }
    if (p < mv) {
      rc = old->rc;
      STree* c = create(lv, mv);
      c->negate(old->lc, p);
      lc = c;
    }
    else {
      lc = old->lc;
      STree* c = create(mv, rv);
      c->negate(old->rc, p);
      rc = c;
    }
    this->val = lc->val + rc->val;
  }

  static STree* cur;
  static STree* create(const unsigned l, const unsigned int r)
  {
    STree* ret = ++cur;
    ret->lv = l;
    ret->rv = r;
    ret->mv = (l + r) >> 1;
    return ret;
  }

 private:
  struct value_type {
    value_type() = default;
    value_type(int pre, int post, int sum) : pre(pre), post(post), sum(sum){};
    value_type(int a) : value_type(a, a, a){};
    value_type operator+(const value_type& r) const
    {
      return value_type(max(pre, sum + r.pre), max(r.post, r.sum + post),
                        sum + r.sum);
    }

    int pre, post, sum;
  } val;
  value_type queryImpl(const unsigned int l, const unsigned int r) const
  {
    if (l == lv && r == rv) return val;
    if (r <= mv)
      return lc->queryImpl(l, r);
    else if (l >= mv)
      return rc->queryImpl(l, r);
    else
      return lc->queryImpl(l, mv) + rc->queryImpl(mv, r);
  }

  unsigned int lv, mv, rv;
  const STree *lc, *rc;
} SNodes[(maxn * 16 + maxn) * 4], *SRoot = SNodes;
const STree* root[maxn];
STree* STree::cur = SNodes - 1;
long long val[maxn + 1], orig[maxn + 1];

static void discretize(const unsigned int n)
{
  copy(val, val + n, orig);
  sort(orig, orig + n);
  long long* e = unique(orig, orig + n);
  for (long long* i = val; i < val + n; ++i)
    *i = lower_bound(orig, e, *i) - orig;
}
static void build(const unsigned int n)
{
  static unsigned int pos[maxn + 1];
  iota(pos, pos + 1 + n, 0);
  sort(pos, pos + n, [](const unsigned int a, const unsigned int b) {
    return val[a] < val[b];
  });
  {
    STree* c = STree::create(0, n);
    c->build(0, n, pos[0]);
    root[0] = c;
  }
  for (unsigned int i = 1; i < n; ++i) {
    STree* c = STree::create(0, n);
    c->negate(root[i - 1], pos[i]);
    root[i] = c;
  }
  for (unsigned int i = 0; i < n; ++i)
    root[val[pos[i]]] = root[i];
}
static void init(const unsigned int n)
{
  discretize(n);
  build(n);
}
long long query(const unsigned int n, const unsigned int l1,
                const unsigned int l2, const unsigned int r1,
                const unsigned int r2)
{
  int l = 0, r = n - 1;
  while (l <= r) {
    const int m = (l + r) >> 1;
    if (root[m]->query(l1, l2, r1, r2) >= 0)
      l = m + 1;
    else
      r = m - 1;
  }
  return orig[l];
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n;
  copy_n(istream_iterator<long long>(cin), n, val);
  init(n);
  cin >> q;

  long long lst = 0;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int q[4];
    cin >> q[0] >> q[1] >> q[2] >> q[3];
#ifndef NoEncode
    transform(q, q + 4, q,
              [lst, n](long long v) -> unsigned int { return (v + lst) % n; });
#endif
    sort(q, q + 4);
    cout << (lst = query(n, q[0], q[1], q[2], q[3])) << endl;
  }
  return 0;
}