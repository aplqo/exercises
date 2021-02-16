#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 35000, maxk = 50;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur),
             unsigned int dat[])
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur, dat);
    rc->build(mv, r, cur, dat);
  }
  void init(const unsigned int dat[])
  {
    laz = 0;
    if (rv - lv == 1) {
      val = dat[lv];
      return;
    }
    lc->init(dat);
    rc->init(dat);
    this->val = max(lc->val, rc->val);
  }
  void increase(const unsigned int l, const unsigned int r)
  {
    if (l == lv && r == rv) {
      push(1);
      return;
    }
    pushdown();
    if (r <= mv)
      lc->increase(l, r);
    else if (l >= mv)
      rc->increase(l, r);
    else {
      lc->increase(l, mv);
      rc->increase(mv, r);
    }
    this->val = max(lc->val, rc->val);
  }
  unsigned int query(const unsigned int l, const unsigned int r)
  {
    if (l == lv && r == rv) return val;
    pushdown();
    if (r <= mv)
      return lc->query(l, r);
    else if (l >= mv)
      return rc->query(l, r);
    else
      return max(lc->query(l, mv), rc->query(mv, r));
  }

 private:
  void push(const unsigned int laz)
  {
    this->val += laz;
    this->laz += laz;
  }
  void pushdown()
  {
    if (!laz) return;
    lc->push(laz);
    rc->push(laz);
    laz = 0;
  }

  STree *lc = nullptr, *rc = nullptr;
  unsigned int val, laz;
  unsigned int lv, mv, rv;
} SNodes[maxn * 4 + 1], *SRoot = SNodes;
unsigned int lst[maxn + 1], a[maxn + 1];
unsigned int f[maxn + 1];

static void initLst(const unsigned int n)
{
  static unsigned int pos[maxn + 1];
  for (unsigned int i = 1; i <= n; ++i) {
    lst[i] = pos[a[i]];
    pos[a[i]] = i;
  }
}
static unsigned int dp(const unsigned int n, const unsigned int k)
{
  for (unsigned int i = 1; i <= n; ++i)
    f[i] = lst[i] ? f[i - 1] : f[i - 1] + 1;
  for (unsigned int i = 2; i <= k; ++i) {
    SRoot->init(f);
    for (unsigned int j = 2; j <= n; ++j) {
      SRoot->increase(lst[j], j);
      f[j] = SRoot->query(1, j);
    }
  }
  return f[n];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  {
    STree* c = SNodes;
    SRoot->build(0, n + 1, c, f);
  }
  initLst(n);
  cout << dp(n, k) << endl;
  return 0;
}