#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 10000, maxq = 10000, maxm = 1000, maxc = 1e6;

struct Modify {
  Modify() = default;
  Modify(const unsigned int p, const unsigned int nv) : pos(p), newv(nv){};
  unsigned int pos;
  unsigned int oldv, newv;
} mod[maxm + 1];
struct Query {
  Query() = default;
  Query(const unsigned int l, const unsigned int r, const unsigned int t)
      : l(l), r(r), t(t){};
  unsigned int l, r, t;
  unsigned int ans = 0;
} qry[maxq + 1];
unsigned int bsize;
unsigned int col[maxn + 1], cnt[maxc + 1];

bool queryCmp(const Query* a, const Query* b)
{
  if (a->l / bsize != b->l / bsize)
    return a->l / bsize < b->l / bsize;
  else
    return a->r != b->r ? a->r < b->r : a->t < b->t;
}
inline unsigned int add(const unsigned int x)
{
  ++cnt[col[x]];
  return cnt[col[x]] == 1;
}
inline unsigned int remove(const unsigned int x)
{
  --cnt[col[x]];
  return !cnt[col[x]];
}
inline unsigned int replace(const unsigned int p, const unsigned int nv,
                            const unsigned int l, const unsigned int r)
{
  if (p < l || p > r) {
    col[p] = nv;
    return 0;
  }
  unsigned int ret = remove(p);
  col[p] = nv;
  ret -= add(p);
  return ret;
}
inline unsigned int apply(const unsigned int tim, const unsigned int l,
                          const unsigned int r)
{
  Modify& m = mod[tim];
  m.oldv = col[m.pos];
  return replace(m.pos, m.newv, l, r);
}
inline unsigned int undo(const unsigned int tim, const unsigned int l,
                         const unsigned int r)
{
  return replace(mod[tim].pos, mod[tim].oldv, l, r);
}
void solve(const unsigned int cq)
{
  static Query* seq[maxq + 1];
  transform(qry, qry + cq, seq, [](Query& i) { return &i; });
  sort(seq, seq + cq, queryCmp);
  unsigned int l = seq[0]->l, r = seq[0]->l, t = 0;
  cnt[col[seq[0]->l]] = 1;
  unsigned int ans = 1;
  for (unsigned int i = 0; i < cq; ++i) {
    Query* const cur = seq[i];
    while (cur->l < l)
      ans += add(--l);
    while (r < cur->r)
      ans += add(++r);
    while (t < cur->t)
      ans -= apply(++t, l, r);
    while (l < cur->l)
      ans -= remove(l++);
    while (r > cur->r)
      ans -= remove(r--);
    while (t > cur->t)
      ans -= undo(t--, l, r);
    cur->ans = ans;
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  copy_n(istream_iterator<unsigned int>(cin), n, col + 1);
  unsigned int cpos = 0, qpos = 0;
  for (unsigned int i = 0; i < m; ++i) {
    char op;
    cin >> op;
    if (op == 'Q') {
      unsigned int l, r;
      cin >> l >> r;
      qry[qpos++] = Query(l, r, cpos);
    }
    else {
      unsigned int p, v;
      cin >> p >> v;
      mod[++cpos] = Modify(p, v);
    }
  }
  bsize = ceil(sqrt(n));
  solve(qpos);
  for (Query* i = qry; i < qry + qpos; ++i)
    cout << i->ans << endl;
  return 0;
}