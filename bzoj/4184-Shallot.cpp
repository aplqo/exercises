#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <forward_list>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>
using namespace std;
const unsigned int maxn = 500000, maxl = 31;

struct Operation {
  unsigned int a;
  bool typ;
  unsigned int ret;
} op[maxn + 1];
forward_list<unsigned int> stk[maxn + 1];
unsigned int orig[maxn + 1];
class LinearBasis {
 public:
  void insert(unsigned int x);
  unsigned int max() const;

 private:
  unsigned int vec[maxl + 1] = {};
};
class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur));
  void add(const unsigned int l, const unsigned int r, const unsigned int v);
  void dfs(LinearBasis lb) const;

 private:
  unsigned int lv, mv, rv;
  STree *lc, *rc;
  forward_list<unsigned int> val;
} SNodes[maxn * 4 + 10], *root = SNodes;

void LinearBasis::insert(unsigned int x)
{
  for (int i = maxl; i >= 0; --i) {
    if (!(x & (1 << i))) continue;
    if (vec[i])
      x ^= vec[i];
    else {
      for (unsigned int j = 0; j < i; ++j)
        if (x & (1 << j)) x ^= vec[j];
      for (unsigned int j = i + 1; j < maxl; ++j)
        if (vec[j] & (1 << i)) vec[j] ^= x;
      vec[i] = x;
      break;
    }
  }
}
unsigned int LinearBasis::max() const
{
  return accumulate(vec, vec + maxl + 1, 0u,
                    [](unsigned int a, unsigned int b) { return a ^ b; });
}

void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (rv - lv == 1) return;
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur);
  rc->build(mv, r, cur);
}
void STree::add(const unsigned int l, const unsigned int r,
                const unsigned int v)
{
  if (l == lv && r == rv) {
    val.push_front(v);
    return;
  }
  if (r <= mv)
    lc->add(l, r, v);
  else if (l >= mv)
    rc->add(l, r, v);
  else {
    lc->add(l, mv, v);
    rc->add(mv, r, v);
  }
}
void STree::dfs(LinearBasis lb) const
{
  for (unsigned int i : val)
    lb.insert(i);
  if (rv - lv == 1) {
    op[lv].ret = lb.max();
    return;
  }
  lc->dfs(lb);
  rc->dfs(lb);
}

static void discretize(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    orig[i] = op[i].a;
  sort(orig, orig + n);
  unsigned int* e = unique(orig, orig + n);
  for (Operation* i = op; i < op + n; ++i)
    i->a = lower_bound(orig, e, i->a) - orig;
}
static void init(const unsigned int n)
{
  {
    STree* c = SNodes;
    root->build(0, n, c);
  }
  discretize(n);
  for (unsigned int i = 0; i < n; ++i) {
    const Operation& o = op[i];
    if (o.typ)
      stk[o.a].push_front(i);
    else {
      root->add(stk[o.a].front(), i, orig[o.a]);
      stk[o.a].pop_front();
    }
  }
  for (unsigned int i = 0; i < n; ++i)
    if (!stk[i].empty()) root->add(stk[i].front(), n, orig[i]);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (Operation* i = op; i < op + n; ++i) {
    int t;
    cin >> t;
    i->typ = t > 0;
    i->a = abs(t);
  }
  init(n);
  root->dfs(LinearBasis());
  for (Operation* i = op; i < op + n; ++i)
    cout << i->ret << endl;
  return 0;
}