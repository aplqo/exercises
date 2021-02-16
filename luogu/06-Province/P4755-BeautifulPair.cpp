#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <set>
using namespace std;
using set_t = multiset<unsigned int>;
const unsigned int maxn = 1e5;

struct node {
  unsigned int val;
  unsigned int lc = 0, rc = 0;
} nod[maxn + 1];
set_t lst, cur;
unsigned int siz[maxn + 1], cnt1;
unsigned long long ans = 0;

static unsigned int build(const unsigned int n)
{
  static unsigned int stk[maxn + 1], *top = stk;
  *(top++) = 1;
  unsigned int root = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int* k = top;
    while (k > stk && nod[*(k - 1)].val < nod[i].val)
      --k;
    if (k > stk)
      nod[*(k - 1)].rc = i;
    else
      root = i;
    if (k < top) nod[i].lc = *k;
    top = k;
    *(top++) = i;
  }
  return root;
}
void dfsSize(const unsigned int x)
{
  if (!x) return;
  dfsSize(nod[x].lc);
  dfsSize(nod[x].rc);
  siz[x] = siz[nod[x].lc] + siz[nod[x].rc] + 1;
}
unsigned long long countSub(const unsigned int v)
{
  unsigned long long ret = 0;
  set_t::reverse_iterator r = lst.rbegin();
  unsigned int cnt = lst.size();
  for (set_t::iterator l = cur.begin(); l != cur.end() && r != lst.rend();
       ++l) {
    for (const unsigned int ex = v / *l; r != lst.rend() && *r > ex; ++r, --cnt)
      ;
    ret += cnt;
  }
  return ret;
}
inline void add(const unsigned int v)
{
  cur.insert(v);
  if (v == 1) ++cnt1;
}
void dfsSubTree(const unsigned int x)
{
  if (!x) return;
  add(nod[x].val);
  dfsSubTree(nod[x].lc);
  dfsSubTree(nod[x].rc);
}
void dfs(const unsigned int x, const bool keep)
{
  if (!x) return;
  if (siz[nod[x].lc] < siz[nod[x].rc]) swap(nod[x].lc, nod[x].rc);
  dfs(nod[x].rc, false);
  dfs(nod[x].lc, true);
  dfsSubTree(nod[x].rc);
  ans += countSub(nod[x].val);
  add(nod[x].val);
  ans += cnt1;
  if (keep)
    lst.merge(cur);
  else {
    cnt1 = 0;
    lst.clear();
  }
  cur.clear();
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (node* i = nod + 1; i < nod + 1 + n; ++i)
    cin >> i->val;
  const unsigned int root = build(n);
  dfsSize(root);
  dfs(root, true);
  cout << ans << endl;
  return 0;
}