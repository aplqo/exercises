#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 100000;

unsigned long long mod;
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
      return;
    }
    lc = ++cur;
    lc->build(l, mv, cur, dat);
    rc = ++cur;
    rc->build(mv, r, cur, dat);
    val = (lc->val + rc->val) % mod;
  }
  void multiply(const unsigned int l, const unsigned int r,
                const unsigned long long val)
  {
    if (l == lv && r == rv) {
      pushm(val);
      return;
    }
    pushdown();
    if (r <= mv)
      lc->multiply(l, r, val);
    else if (l >= mv)
      rc->multiply(l, r, val);
    else {
      lc->multiply(l, mv, val);
      rc->multiply(mv, r, val);
    }
    this->val = (lc->val + rc->val) % mod;
  }
  void plus(const unsigned int l, const unsigned int r,
            const unsigned long long val)
  {
    if (l == lv && r == rv) {
      pusha(val);
      return;
    }
    pushdown();
    if (r <= mv)
      lc->plus(l, r, val);
    else if (l >= mv)
      rc->plus(l, r, val);
    else {
      lc->plus(l, mv, val);
      rc->plus(mv, r, val);
    }
    this->val = (lc->val + rc->val) % mod;
  }
  unsigned long long query(const unsigned int l, const unsigned int r)
  {
    if (l == lv && r == rv) return val;
    pushdown();
    if (r <= mv)
      return lc->query(l, r);
    else if (l >= mv)
      return rc->query(l, r);
    else
      return (lc->query(l, mv) + rc->query(mv, r)) % mod;
  }

 private:
  void pushm(const unsigned long long laz)
  {
    laza = (laza * laz) % mod;
    lazm = (lazm * laz) % mod;
    val = (val * laz) % mod;
  }
  void pusha(const unsigned long long laz)
  {
    laza = (laza + laz) % mod;
    val = (val + ((rv - lv) * laz) % mod) % mod;
  }
  void pushdown()
  {
    if (lazm == 1 && !laza) return;
    if (lazm != 1) {
      lc->pushm(lazm);
      rc->pushm(lazm);
      lazm = 1;
    }
    lc->pusha(laza);
    rc->pusha(laza);
    laza = 0;
  }

  unsigned int lv, rv, mv;
  STree *lc = nullptr, *rc = nullptr;
  unsigned long long val;
  unsigned long long lazm = 1, laza = 0;
} SNode[maxn * 4 + 10], *SRoot = SNode;
unsigned long long dat[maxn + 1];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> mod;
  copy_n(istream_iterator<unsigned long long>(cin), n, dat + 1);
  cin >> q;
  {
    STree* c = SNode;
    SRoot->build(1, n + 1, c, dat);
  }
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int op, l, r;
    unsigned long long v;
    cin >> op >> l >> r;
    switch (op) {
      case 1:
        cin >> v;
        SRoot->multiply(l, r + 1, v);
        break;
      case 2:
        cin >> v;
        SRoot->plus(l, r + 1, v);
        break;
      case 3:
        cout << SRoot->query(l, r + 1) << endl;
        break;
    }
  }
  return 0;
}