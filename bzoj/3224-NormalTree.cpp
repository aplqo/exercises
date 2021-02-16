#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <utility>
using namespace std;
const int maxn = 1e5, maxv = 1e7;

class STree {
 public:
  void build(const int l, const int r)
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    lc = rc = nullptr;
    val = 0;
  }
  void modify(const int p, const int val)
  {
    if (rv - lv == 1) {
      this->val += val;
      return;
    }
    if (p < mv)
      create(lc, lv, mv)->modify(p, val);
    else
      create(rc, mv, rv)->modify(p, val);
    this->val = get(lc) + get(rc);
  }
  int rank(const int x) const { return query(x) + 1; }
  int find(const int p) const
  {
    if (rv - lv == 1) return lv;
    return p <= get(lc) ? lc->find(p) : rc->find(p - get(lc));
  }
  int pre(const int x) const { return find(query(x)); }
  int nxt(const int x) const { return find(query(x + 1) + 1); }

  static STree* cur;

 private:
  int query(const int x) const  // get sum of [-maxx,x)
  {
    if (x == rv || rv - lv == 1) return val;
    const auto qry = [](const STree* s, const int x) -> int {
      return s ? s->query(x) : 0;
    };
    return x <= mv ? qry(lc, x) : get(lc) + qry(rc, x);
  }

  static inline int get(const STree* c) { return c ? c->val : 0; }
  static inline STree* create(STree*(&c), const int l, const int r)
  {
    if (!c) {
      c = ++cur;
      c->build(l, r);
    }
    return c;
  }

  int lv, mv, rv;
  STree *lc, *rc;
  int val;
} SNodes[maxn * 40], *SRoot = SNodes;
STree* STree::cur = SNodes;

int main(int argc, char* argv[])
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
#ifdef APDEBUG
  if (argc > 1) freopen(argv[1], "r", stdin);
#endif
  unsigned int n;
  cin >> n;
  SRoot->build(-maxv - 2, maxv + 2);
  for (unsigned int i = 0; i < n; ++i) {
    int opt, x;
    cin >> opt >> x;
    switch (opt) {
      case 1:
        SRoot->modify(x, 1);
        break;
      case 2:
        SRoot->modify(x, -1);
        break;
      case 3:
        cout << SRoot->rank(x) << endl;
        break;
      case 4:
        cout << SRoot->find(x) << endl;
        break;
      case 5:
        cout << SRoot->pre(x) << endl;
        break;
      case 6:
        cout << SRoot->nxt(x) << endl;
        break;
    }
  }
  return 0;
}