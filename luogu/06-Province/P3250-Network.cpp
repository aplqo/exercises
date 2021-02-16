#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxm = 2e5, maxw = 1e9;
template <class T>
using Range = pair<T, T>;

struct Edge {
  unsigned int to;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
unsigned int dfn[maxn + 10], siz[maxn + 10], fa[maxn + 10];
struct Connect {
  unsigned int x, y, lca;
  unsigned int weight;
} con[maxm + 10];
struct Event {
  enum { Add, Remove, Query } typ;
  union {
    const Connect* con;
    struct {
      unsigned int pos, val;
    } qry;
  } val;
} eve[maxm + 10];
Event* q[maxm + 10];
class FTree {
 public:
  void add(unsigned int p, const int delt);
  unsigned int query(const unsigned int l, const unsigned int r) const;
  void clear();

  unsigned int siz;

 private:
  int preSum(unsigned int pos) const;

  vector<unsigned int> stk;
  bool vis[maxn + 10]{};
  int val[maxn + 10]{};
} ft;
namespace Lca {
unsigned int dep[maxn + 10], son[maxn + 10], top[maxn + 10];

void dfsSon(const unsigned int x, const unsigned int f);
void dfsChain(const unsigned int x, const unsigned int tp);
static void init();
unsigned int lca(unsigned int a, unsigned int b);
}  // namespace Lca

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, head[to]};
  head[to] = cur++;
}

#define lowbit(x) ((x) & -(x))
void FTree::add(unsigned int pos, const int delt)
{
  if (!pos) return;
  for (; pos <= siz; pos += lowbit(pos)) {
    val[pos] += delt;
    if (!vis[pos]) {
      vis[pos] = true;
      stk.push_back(pos);
    }
  }
}
unsigned int FTree::query(const unsigned int l, const unsigned int r) const
{
  return preSum(r - 1) - preSum(l - 1);
}
void FTree::clear()
{
  for (unsigned int j : stk) {
    val[j] = 0;
    vis[j] = false;
  }
  stk.clear();
}
int FTree::preSum(unsigned int pos) const
{
  int ret = 0;
  for (; pos; pos -= lowbit(pos))
    ret += val[pos];
  return ret;
}
#undef lowbit

namespace Lca {
void dfsSon(const unsigned int x, const unsigned int f)
{
  fa[x] = f;
  dep[x] = dep[f] + 1;
  siz[x] = 1;
  unsigned int ms = 0;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    dfsSon(to, x);
    siz[x] += siz[to];
    if (siz[to] > ms) {
      ms = siz[to];
      son[x] = to;
    }
  }
}
void dfsChain(const unsigned int x, const unsigned int tp)
{
  static unsigned int sta = 0;
  dfn[x] = ++sta;
  top[x] = tp;
  if (son[x]) dfsChain(son[x], tp);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (!dfn[i->to]) dfsChain(i->to, i->to);
}
static void init()
{
  dfsSon(1, 0);
  dfsChain(1, 1);
}
unsigned int lca(unsigned int a, unsigned int b)
{
  while (top[a] != top[b]) {
    if (dep[top[a]] > dep[top[b]]) swap(a, b);
    b = fa[top[b]];
  }
  return dep[a] < dep[b] ? a : b;
}
}  // namespace Lca

inline unsigned int applyEvent(
    const Event* e, const int active)  // return new active connections
{
  const Connect* co = e->val.con;
  const int delt = e->typ == Event::Remove ? -1 : 1;
  ft.add(dfn[co->x], delt);
  ft.add(dfn[co->y], delt);
  ft.add(dfn[co->lca], -delt);
  ft.add(dfn[fa[co->lca]], -delt);
  return active + delt;
}
inline unsigned int queryVertex(const unsigned int x)
{
  return ft.query(dfn[x], dfn[x] + siz[x]);
}

typedef Range<Event**> QRange;
typedef Range<unsigned int> VRange;
void binary(const QRange er, const VRange vr)
{
  static Event *ql[maxm + 10], *qr[maxm + 10];
  if (vr.second - vr.first == 1) {
    for (Event** i = er.first; i < er.second; ++i)
      if ((*i)->typ == Event::Query) (*i)->val.qry.val = vr.first;
    return;
  }
  const unsigned int mid = (vr.first + vr.second) >> 1;
  unsigned int pl = 0, pr = 0, active = 0;
  for (Event** i = er.first; i < er.second; ++i) {
    Event* const e = *i;
    if (e->typ == Event::Add || e->typ == Event::Remove) {
      if (e->val.con->weight < mid)
        ql[pl++] = e;
      else {
        qr[pr++] = e;
        active = applyEvent(e, active);
      }
    }
    else {
      if (queryVertex(e->val.qry.pos) < active)
        qr[pr++] = e;
      else
        ql[pl++] = e;
    }
  }
  ft.clear();
  copy(ql, ql + pl, er.first);
  copy(qr, qr + pr, er.first + pl);
  if (pl) binary(QRange{er.first, er.first + pl}, VRange{vr.first, mid});
  if (pr) binary(QRange{er.first + pl, er.second}, VRange{mid, vr.second});
}
void readEvents(const unsigned int m)
{
  Connect* pc = con;
  for (unsigned int i = 0; i < m; ++i) {
    Event& ce = eve[i];
    unsigned int typ;
    cin >> typ;
    switch (typ) {
      case 0: {
        cin >> pc->x >> pc->y >> pc->weight;
        pc->lca = Lca::lca(pc->x, pc->y);
        ce.val.con = pc++;
        ce.typ = Event::Add;
        break;
      }
      case 1: {
        unsigned int t;
        cin >> t;
        ce.typ = Event::Remove;
        ce.val.con = eve[t - 1].val.con;
        break;
      }
      case 2:
        cin >> ce.val.qry.pos;
        ce.typ = Event::Query;
        break;
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
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v);
  }
  Lca::init();
  ft.siz = n;
  readEvents(m);
  transform(eve, eve + m, q, [](Event& i) { return &i; });
  binary(Range<Event**>{q, q + m}, Range<unsigned int>{0, maxw + 1});
  for (const Event* i = eve; i < eve + m; ++i)
    if (i->typ == Event::Query) {
      const unsigned int v = i->val.qry.val;
      if (v > 0 && v <= maxw)
        cout << v << "\n";
      else
        cout << "-1\n";
    }
  return 0;
}