#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <stack>
#include <unordered_map>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxq = 1e5, maxv = 10;
constexpr unsigned int inf = UINT_MAX;

struct Query {
  unsigned int x, y, ret;
} qry[maxq + 10];
struct Edge {
  unsigned int from, to, val;
  unsigned int begin, end;
} ed[maxq + 10];
class Disjoint {
 public:
  void init(const unsigned int n);
  void merge(unsigned int x, unsigned int y);
  inline bool isConnect(const unsigned int x, const unsigned int y) const;
  unsigned int save() const { return stk.size(); }
  void revert(const unsigned int stat);

 private:
  unsigned int find(unsigned int x) const;

  unsigned int fa[maxn + 10], siz[maxn + 10];
  stack<unsigned int> stk;
} dsu[maxv + 1];
class SegmentTree {
 public:
  void build(const unsigned int l, const unsigned int r, SegmentTree*(&cur));
  void insert(const unsigned int l, const unsigned int r, const Edge* e);
  void dfs() const;

 private:
  unsigned int lv, mv, rv;
  SegmentTree *lc, *rc;
  vector<const Edge*> ed;
} snodes[maxq * 4 + 10], *root = snodes;

void Disjoint::init(const unsigned int n)
{
  iota(fa, fa + 1 + n, 0);
  fill(siz, siz + 1 + n, 1);
}
void Disjoint::merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
  stk.push(y);
}
inline bool Disjoint::isConnect(const unsigned int x,
                                const unsigned int y) const
{
  return find(x) == find(y);
}
void Disjoint::revert(const unsigned int stat)
{
  while (stk.size() != stat) {
    const unsigned int v = stk.top();
    stk.pop();
    siz[fa[v]] -= siz[v];
    fa[v] = v;
  }
}
unsigned int Disjoint::find(unsigned int x) const
{
  while (fa[x] != x)
    x = fa[x];
  return x;
}

void SegmentTree::build(const unsigned int l, const unsigned int r,
                        SegmentTree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  if (r - l == 1) return;
  lc = ++cur;
  lc->build(l, mv, cur);
  rc = ++cur;
  rc->build(mv, r, cur);
}
void SegmentTree::insert(const unsigned int l, const unsigned int r,
                         const Edge* e)
{
  if (l == lv && r == rv) {
    ed.push_back(e);
    return;
  }
  if (r <= mv)
    lc->insert(l, r, e);
  else if (l >= mv)
    rc->insert(l, r, e);
  else {
    lc->insert(l, mv, e);
    rc->insert(mv, r, e);
  }
}
void SegmentTree::dfs() const
{
  unsigned int stat[maxv];
  for (unsigned int i = 0; i < maxv; ++i)
    stat[i] = dsu[i].save();
  for (const Edge* const i : ed)
    for (unsigned int j = i->val; j < maxv; ++j)
      dsu[j].merge(i->from, i->to);
  if (rv - lv == 1) {
    Query& q = qry[lv];
    for (unsigned int i = 0; i < maxv; ++i)
      if (dsu[i].isConnect(q.x, q.y)) {
        q.ret = i;
        break;
      }
  }
  else {
    lc->dfs();
    rc->dfs();
  }
  for (unsigned int i = 0; i < maxv; ++i)
    dsu[i].revert(stat[i]);
}

pair<unsigned int, const Edge*> readQuery(const unsigned int n,
                                          const unsigned int q)
{
  static unordered_map<unsigned int, Edge*> edges[maxn + 10];
  Edge* cur = ed;
  unsigned int qcur = 1;
  for (unsigned int i = 1; i <= q; ++i) {
    unsigned int op, x, y, v;
    cin >> op >> x >> y;
    switch (op) {
      case 0:
        cin >> v;
        edges[x][y] = edges[y][x] = cur;
        new (cur++) Edge{x, y, v, qcur, 0};
        break;
      case 1:
        edges[x][y]->end = qcur;
        break;
      case 2:
        new (qry + qcur) Query{x, y, inf};
        ++qcur;
        break;
    }
  }
  return make_pair(qcur, cur);
}
static unsigned int init(const unsigned int n, const unsigned int q)
{
  const auto [qcur, ecur] = readQuery(n, q);
  {
    SegmentTree* c = snodes;
    root->build(1, qcur + 1, c);
  }
  for (const Edge* i = ed; i < ecur; ++i)
    if (i->begin != i->end)
      root->insert(i->begin, i->end ? i->end : qcur + 1, i);
  for (Disjoint* i = dsu; i < dsu + maxv; ++i)
    i->init(n);
  return qcur;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, q;
  cin >> n >> q;
  const unsigned int qc = init(n, q);
  root->dfs();
  for (const Query* i = qry + 1; i < qry + qc; ++i)
    if (i->ret == inf)
      cout << "-1\n";
    else
      cout << i->ret << "\n";
  return 0;
}