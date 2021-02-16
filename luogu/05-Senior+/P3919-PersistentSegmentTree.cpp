#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iterator>
#include <sstream>
using namespace std;
const unsigned int maxn = 1e6;

template <class T>
T read()
{
  char c = getchar();
  T ret = 0, r = 1;
  while (!isdigit(c) && c != '-')
    c = getchar();
  if (c == '-') {
    r = -1;
    c = getchar();
  }
  while (isdigit(c)) {
    ret = ret * 10 + c - '0';
    c = getchar();
  }
  return ret * r;
}
class STree {
 public:
  void build(const unsigned int l, const unsigned int r, int dat[])
  {
    if (r - l == 1) {
      val = dat[l];
      return;
    }
    STree* c = create(l, mv);
    c->build(l, mv, dat);
    lc = c;
    c = create(mv, r);
    c->build(mv, r, dat);
    rc = c;
  }
  void assign(const STree* old, const unsigned int x, const int val)
  {
    if (rv - lv == 1) {
      this->val = val;
      return;
    }
    if (x < mv) {
      rc = old->rc;
      STree* c = create(lv, mv);
      c->assign(old->lc, x, val);
      lc = c;
    }
    else {
      lc = old->lc;
      STree* c = create(mv, rv);
      c->assign(old->rc, x, val);
      rc = c;
    }
  }
  int find(const unsigned int x) const
  {
    if (rv - lv == 1) return val;
    return x < mv ? lc->find(x) : rc->find(x);
  }
  static STree* create(const unsigned int l, const unsigned int r)
  {
    ++cur;
    cur->lv = l;
    cur->rv = r;
    cur->mv = (l + r) >> 1;
    return cur;
  }

 private:
  static STree* cur;

  unsigned int lv, mv, rv;
  int val;
  const STree *lc, *rc;
} SNodes[maxn * 4 + maxn * 10 * 4];
STree* STree::cur = SNodes - 1;
const STree* root[maxn + 1];
int seq[maxn + 1];

int main()
{
  const unsigned int n = read<unsigned int>();
  const unsigned int m = read<unsigned int>();
  generate_n(seq + 1, n, read<int>);
  ostringstream os;
  {
    STree* c = STree::create(1, n + 1);
    c->build(1, n + 1, seq);
    root[0] = c;
  }
  for (unsigned int i = 1; i <= m; ++i) {
    const unsigned int v = read<unsigned int>();
    const unsigned int op = read<unsigned int>();
    const unsigned int x = read<unsigned int>();
    if (op == 1) {
      const int val = read<int>();
      STree* c = STree::create(1, n + 1);
      c->assign(root[v], x, val);
      root[i] = c;
    }
    else {
      root[i] = root[v];
      os << root[v]->find(x) << endl;
    }
  }
  puts(os.str().c_str());
  return 0;
}