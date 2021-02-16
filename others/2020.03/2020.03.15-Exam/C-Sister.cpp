#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <type_traits>
using namespace std;
const unsigned int maxn = 1e6;

class node {
 public:
  enum typ { MAX = 0, MIN = 1 };
  void build(const unsigned int l, const unsigned int r, unsigned int dat[],
             node*(&cur))
  {
    lv = l;
    rv = r;
    if (r - l == 1) {
      val = dat[l];
      m[MAX] = m[MIN] = dat[l];
      return;
    }
    mv = (l + r) >> 1;
    lc = ++cur;
    lc->build(lv, mv, dat, cur);
    rc = ++cur;
    rc->build(mv, rv, dat, cur);
    getinfo();
  }
  void add(const unsigned int l, const unsigned int r, const unsigned int val)
  {
    if ((l == lv && r == rv) || rv - lv == 1) {
      pusha(val);
      return;
    }
    push();
    if (r <= mv)
      lc->add(l, r, val);
    else if (l >= mv)
      rc->add(l, r, val);
    else {
      lc->add(l, mv, val);
      rc->add(mv, r, val);
    }
    getinfo();
  }
  void replace(const unsigned int l, const unsigned int r, const unsigned val)
  {
    if ((l == lv && r == rv) || rv - lv == 1) {
      pushr(val);
      return;
    }
    push();
    if (r <= mv)
      lc->replace(l, r, val);
    else if (l >= mv)
      rc->replace(l, r, val);
    else {
      lc->replace(l, mv, val);
      rc->replace(mv, r, val);
    }
    getinfo();
  }
  template <unsigned int t>
  unsigned int get_maxmin(const unsigned int l, const unsigned int r)
  {
    if ((l == lv && r == rv) || rv - lv == 1) return m[t];
    push();
    if (r <= mv)
      return lc->get_maxmin<t>(l, r);
    else if (l >= mv)
      return rc->get_maxmin<t>(l, r);
    else {
      const unsigned int lans = lc->get_maxmin<t>(l, mv),
                         rans = rc->get_maxmin<t>(mv, r);
      conditional_t<t == MIN, less<unsigned int>, greater<unsigned int>> cmp;
      return cmp(lans, rans) ? lans : rans;
    }
  }
  unsigned int get_sum(const unsigned int l, const unsigned int r)
  {
    if ((l == lv && r == rv) || rv - lv == 1) return val;
    push();
    if (r <= mv)
      return lc->get_sum(l, r);
    else if (l >= mv)
      return rc->get_sum(l, r);
    else
      return lc->get_sum(l, mv) + rc->get_sum(mv, r);
  }

 private:
  inline void pushr(const unsigned int laz)
  {
    rep = true;
    lazr = laz;
    laza = 0;
    val = laz * (rv - lv);
    m[MAX] = m[MIN] = laz;
  }
  inline void pusha(const unsigned int laz)
  {
    laza += laz;
    val += laz * (rv - lv);
    m[MAX] += laz;
    m[MIN] += laz;
  }
  inline void push()
  {
    bool upd = laza || rep;
    if (rep) {
      lc->pushr(lazr);
      rc->pushr(lazr);
      lazr = 0;
      rep = false;
    }
    if (laza) {
      lc->pusha(laza);
      rc->pusha(laza);
      laza = 0;
    }
    if (upd) getinfo();
  }
  inline void getinfo()
  {
    val = lc->val + rc->val;
    m[MAX] = max(lc->m[MAX], rc->m[MAX]);
    m[MIN] = min(lc->m[MIN], rc->m[MIN]);
  }
  node *lc = nullptr, *rc = nullptr;
  unsigned int lv, rv, mv;
  unsigned int laza = 0, lazr = 0;
  bool rep = false;
  unsigned int val, m[2];
} tr[maxn * 4 + 10], *tree = tr;
unsigned int dat[maxn + 10];

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int* i = dat; i < dat + n; ++i)
    cin >> *i;
  {
    node* c = tr;
    tree->build(0, n, dat, c);
  }
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int c, dir, l, r;
    cin >> c >> dir >> l >> r;
    if (c == 1) {
      switch (dir) {
        case 1:
          cout << tree->get_sum(l - 1, r) << endl;
          break;
        case 2:
          cout << tree->get_maxmin<node::MIN>(l - 1, r) << endl;
          break;
        case 3:
          cout << tree->get_maxmin<node::MAX>(l - 1, r) << endl;
          break;
      }
    }
    else {
      unsigned int val;
      cin >> val;
      if (dir == 1)
        tree->add(l - 1, r, val);
      else
        tree->replace(l - 1, r, val);
    }
  }
  return 0;
}