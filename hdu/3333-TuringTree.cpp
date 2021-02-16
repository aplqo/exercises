#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <unordered_map>
using namespace std;
#define lowbit(x) ((x) & -(x))
const int maxn = 30000, maxq = 100000;

struct query {
  unsigned int l, r;
  unsigned long long val;
} qry[maxq];
query* seq[maxq + 1];
unordered_map<long long, unsigned int> sel;
long long val[maxn + 1];

istream& operator>>(istream& is, query& v)
{
  is >> v.l >> v.r;
  return is;
}
ostream& operator<<(ostream& os, const query& q)
{
  os << q.val;
  return os;
}
namespace tree {
long long tree[maxn + 1];
inline void init() { memset(tree, 0, sizeof(tree)); }
long long query(unsigned int p)
{
  long long ret = 0;
  for (; p; p -= lowbit(p))
    ret += tree[p];
  return ret;
}
void modify(unsigned int p, const unsigned int n, long long val)
{
  for (; p <= n; p += lowbit(p))
    tree[p] += val;
}
inline long long sum(const unsigned int l, const unsigned int r)
{
  return query(r) - query(l - 1);
}
}  // namespace tree

inline void insert(const unsigned int n, const unsigned int p)
{
  auto it = sel.find(val[p]);
  if (it == sel.end()) {
    sel[val[p]] = p;
    tree::modify(p, n, val[p]);
    return;
  }
  if (it->second >= p) return;
  tree::modify(it->second, n, -val[p]);
  tree::modify(p, n, val[p]);
  it->second = p;
}
void solveQuery(const unsigned int n, const unsigned int q)
{
  unsigned int pr = 1;
  for (unsigned int i = 0; i < q; ++i) {
    while (pr <= seq[i]->r)
      insert(n, pr++);
    if (seq[i]->l == seq[i]->r) {
      seq[i]->val = val[seq[i]->l];
      continue;
    }
    seq[i]->val = tree::sum(seq[i]->l, seq[i]->r);
  }
}
void solve()
{
  unsigned int n, q;
  cin >> n;
  sel.reserve(n);
  copy_n(istream_iterator<long long>(cin), n, val + 1);
  cin >> q;
  copy_n(istream_iterator<query>(cin), q, qry);
  transform(qry, qry + q, seq, [](query& i) { return &i; });
  sort(seq, seq + q, [](query* a, query* b) {
    return a->r != b->r ? a->r < b->r : a->l < b->l;
  });
  solveQuery(n, q);
  copy(qry, qry + q, ostream_iterator<query>(cout, "\n"));
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i) {
    solve();
    sel.clear();
    tree::init();
  }
  return 0;
}