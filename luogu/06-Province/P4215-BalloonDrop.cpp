#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5, maxa = 1e5, maxl = 17;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r)
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    val = 0;
    lc = rc = nullptr;
  }
  void increase(const unsigned int p)
  {
    if (rv - lv == 1) {
      ++val;
      return;
    }
    if (p < mv)
      create(lc, lv, mv)->increase(p);
    else
      create(rc, mv, rv)->increase(p);
    pushup();
  }
  unsigned int count(const unsigned int p) const
  {
    if (rv - lv == 1) return val;
    if (p < mv)
      return (lc ? lc->count(p) : 0) + get(rc);
    else
      return rc ? rc->count(p) : 0;
  }
  void remove(const unsigned int p)
  {
    if (p < mv) {
      rc = nullptr;
      if (lc) lc->remove(p);
    }
    else if (rc)
      rc->remove(p);
    pushup();
  }
  void merge(STree* const other)
  {
    if (!other) return;
    if (rv - lv == 1) {
      this->val += other->val;
      return;
    }
    lc = may(lc, other->lc);
    rc = may(rc, other->rc);
    pushup();
  }

  static STree* cur;

 private:
  inline void pushup() { this->val = get(lc) + get(rc); }
  static STree* create(STree*(&c), const unsigned int l, const unsigned int r)
  {
    if (c) return c;
    c = ++cur;
    c->build(l, r);
    return c;
  }
  static unsigned int get(STree* c) { return c ? c->val : 0; }
  static STree* may(STree* a, STree*(&other))
  {
    if (!a) {
      a = other;
      other = nullptr;
      return a;
    }
    a->merge(other);
    return a;
  }

  unsigned int lv, mv, rv;
  STree *lc, *rc;
  unsigned int val;
} SNodes[(maxn + 1 + maxn * maxl + maxm * 2) * 4];
STree* root[maxn + 1];
STree* STree::cur;
unsigned int fa[maxn + 1];
unsigned int a[maxn + 1];

unsigned int getFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = getFather(fa[x]);
}
static void init(const unsigned int n)
{
  STree::cur = SNodes + n;
  iota(fa, fa + n + 1, 0);
  {
    root[0] = SNodes;
    root[0]->build(1, n + 1);
  }
  for (unsigned int i = 1; i <= n; ++i) {
    if (!a[i]) {
      fa[i] = i - 1;
      continue;
    }
    root[i] = SNodes + i;
    root[i]->build(1, n + 1);
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, q;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  init(n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int l, r;
    cin >> l >> r;
    root[r]->increase(l);
  }
  cin >> q;

  unsigned int lst = 0;
  for (unsigned int i = 0; i < q; ++i) {
    unsigned int x;
    cin >> x;
#ifndef NoEncode
    x = (x + lst - 1) % n + 1;
#endif
    --a[x];
    if (!a[x]) {
      const unsigned int f = getFather(x - 1);
      cout << (lst += root[x]->count(f + 1)) << endl;
      root[x]->remove(f + 1);
      root[f]->merge(root[x]);
      fa[x] = f;
    }
    else
      cout << lst << endl;
  }
  return 0;
}