#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <stack>
#include <vector>
using namespace std;
const unsigned int maxn = 5000, maxm = 5e5;

unsigned int n;
namespace unf {
unsigned int fa[maxn + 1], siz[maxn + 1];
stack<unsigned int> stk;

void init(const unsigned int n)
{
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
}
unsigned int getFather(const unsigned int x)
{
  return fa[x] == x ? x : getFather(fa[x]);
}
void merge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = getFather(x), fy = getFather(y);
  if (fx == fy) return;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
  stk.push(fy);
}
size_t getState() { return stk.size(); }
void undo(const size_t s)
{
  while (stk.size() > s) {
    const unsigned int c = stk.top();
    stk.pop();
    siz[fa[c]] -= siz[c];
    fa[c] = c;
  }
}
}  // namespace unf
unsigned int pre[(maxn + 1) * (maxn + 1) + 1];
struct Query {
  unsigned int x, y;
  bool vis = false, ans;
} qry[maxm + 1];
class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur))
  {
    lv = l;
    rv = r;
    mv = (l + r) >> 1;
    if (r - l == 1) return;
    lc = ++cur;
    rc = ++cur;
    lc->build(l, mv, cur);
    rc->build(mv, r, cur);
  }
  void add(const unsigned int l, const unsigned int r, const unsigned int v)
  {
    if (l == lv && r == rv) {
      val.push_back(v);
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
  void dfs()
  {
    const size_t s = unf::getState();
    for (unsigned int i : val) {
      const unsigned int x = i / n, y = i % n;
      unf::merge(x, y);
    }
    if (rv - lv == 1) {
      Query& q = qry[lv];
      if (q.vis) q.ans = unf::getFather(q.x) == unf::getFather(q.y);
      unf::undo(s);
      return;
    }
    lc->dfs();
    rc->dfs();
    unf::undo(s);
  }

 private:
  unsigned int lv, mv, rv;
  STree *lc, *rc;
  vector<unsigned int> val;
} * SNode, *SRoot = SNode;

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int m, cur = 1;
  cin >> n >> m;
  {
    SRoot = SNode = new STree[(m + 1) * 4 + 5];
    STree* c = SNode;
    SRoot->build(0, m, c);
  }
  unf::init(n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int op, x, y;
    cin >> op >> x >> y;
    if (x > y) swap(x, y);
    --x;
    --y;
    switch (op) {
      case 0:
        pre[x * n + y] = cur;
        break;
      case 1: {
        const unsigned int e = x * n + y;
        if (pre[e] != cur) SRoot->add(pre[e], cur, e);
        pre[e] = 0;
        break;
      }
      case 2: {
        qry[cur].x = x;
        qry[cur].y = y;
        qry[cur++].vis = true;
        break;
      }
    }
  }
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = i + 1; j < n; ++j) {
      unsigned int e = i * n + j;
      if (pre[e] && pre[e] < cur) SRoot->add(pre[e], cur, e);
    }
  SRoot->dfs();
  for (unsigned int i = 1; i < cur; ++i)
    cout << (qry[i].ans ? "Y" : "N") << endl;
  return 0;
}