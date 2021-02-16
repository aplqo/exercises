#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxq = 1e7 * 2, maxn = 2e5, maxm = 5e4;

class STree {
 public:
  void build(const unsigned int l, const unsigned int r, STree*(&cur));
  void reset(const unsigned int l, const unsigned int r, const unsigned int st);
  void dump(const unsigned int l, const unsigned int r, unsigned int buf[]);

 private:
  void push(const unsigned int v);
  void pushdown();

  unsigned int lv, mv, rv;
  STree *lc, *rc;
  unsigned int val;
  bool valid;
} SNodes[maxn * 4 + 10], *root = SNodes;
long long ans[maxm + 1], *cans = ans;
struct QueryPoint {
  unsigned int pos, sta;
  long long rat;
  long long* ret;
} qry[maxq * 2 + 10], *cur = qry;
struct Operate {
  enum { Null, Merge, Add, Reset, Sum } typ = Null;
  unsigned int ar1;
  union {
    unsigned int ui;
    long long ll;
  } ar2;
} op[maxm + 1];
namespace dsu {
unsigned int fa[maxn + 1], siz[maxn + 1];
long long acc[maxn + 1], val[maxn + 1];
vector<unsigned int> chl[maxn + 1];

static void init(const unsigned int n)
{
  fill(siz, siz + 1 + n, 1);
  iota(fa, fa + 1 + n, 0);
  for (unsigned int i = 1; i <= n; ++i)
    chl[i].push_back(i);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
inline long long query(const unsigned int x) { return val[x] + acc[find(x)]; }
void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  for (unsigned int i : chl[y]) {
    val[i] = val[i] + acc[y] - acc[x];
    chl[x].push_back(i);
  }
  chl[y].clear();
  fa[y] = x;
  siz[x] += siz[y];
}
inline void add(const unsigned int x, const long long v) { acc[find(x)] += v; }
}  // namespace dsu
void STree::build(const unsigned int l, const unsigned int r, STree*(&cur))
{
  lv = l;
  rv = r;
  mv = (l + r) >> 1;
  val = 1;
  valid = true;
  if (r - l == 1) return;
  lc = ++cur;
  rc = ++cur;
  lc->build(l, mv, cur);
  rc->build(mv, r, cur);
}
void STree::dump(const unsigned int l, const unsigned int r, unsigned int buf[])
{
  if (valid) {
    fill(buf + l, buf + r, val);
    return;
  }
  if (r <= mv)
    lc->dump(l, r, buf);
  else if (l >= mv)
    rc->dump(l, r, buf);
  else {
    lc->dump(l, mv, buf);
    rc->dump(mv, r, buf);
  }
}
void STree::reset(const unsigned int l, const unsigned int r,
                  const unsigned int s)
{
  if (l == lv && r == rv) {
    push(s);
    return;
  }
  pushdown();
  if (r <= mv)
    lc->reset(l, r, s);
  else if (l >= mv)
    rc->reset(l, r, s);
  else {
    lc->reset(l, mv, s);
    rc->reset(mv, r, s);
  }
}
void STree::push(const unsigned int v)
{
  val = v;
  valid = true;
}
void STree::pushdown()
{
  if (!valid) return;
  lc->push(val);
  rc->push(val);
  valid = false;
}

inline void addQuery(const unsigned int sta, const unsigned int l,
                     const unsigned int r)
{
  static unsigned int rst[maxn + 1];
  root->dump(l, r + 1, rst);
  for (unsigned int i = l; i < r; ++i) {
    *(cur++) = QueryPoint{i, rst[i] - 1, -1, cans};
    *(cur++) = QueryPoint{i, sta, 1, cans};
  }
  cans++;
}
void solveQuery(const unsigned int m)
{
  sort(qry, cur,
       [](const QueryPoint& a, const QueryPoint& b) { return a.sta < b.sta; });
  QueryPoint* p = qry;
  for (unsigned int i = 0; i <= m; ++i) {
    switch (op[i].typ) {
      case Operate::Merge:
        dsu::merge(op[i].ar1, op[i].ar2.ui);
        break;
      case Operate::Add:
        dsu::add(op[i].ar1, op[i].ar2.ll);
        break;
      default:
        break;
    }
    while (p < cur && p->sta == i) {
      *(p->ret) += p->rat * dsu::query(p->pos);
      ++p;
    }
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  dsu::init(n);
  {
    STree* c = SNodes;
    c->build(1, n + 1, c);
  }
  for (unsigned int i = 1; i <= m; ++i) {
    unsigned int o;
    cin >> o;
    switch (o) {
      case 1:
        op[i].typ = Operate::Merge;
        cin >> op[i].ar1 >> op[i].ar2.ui;
        break;
      case 2:
        op[i].typ = Operate::Add;
        cin >> op[i].ar1 >> op[i].ar2.ll;
        break;
      case 3:
        op[i].typ = Operate::Reset;
        cin >> op[i].ar1 >> op[i].ar2.ui;
        root->reset(op[i].ar1, op[i].ar2.ui + 1, i);
        break;
      case 4:
        op[i].typ = Operate::Sum;
        cin >> op[i].ar1 >> op[i].ar2.ui;
        addQuery(i, op[i].ar1, op[i].ar2.ui + 1);
        break;
    }
  }
  solveQuery(m);
  copy(ans, cans, ostream_iterator<long long>(cout, "\n"));
  return 0;
}