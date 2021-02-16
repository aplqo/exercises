#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned int maxn = 2e5;

class Array {
 public:
  inline void push_back(const unsigned int x) { *(cur++) = x; }
  inline const unsigned int* begin() const { return dat; }
  inline const unsigned int* end() const { return cur; }
  inline void clear() { cur = dat; }

 private:
  unsigned int dat[maxn + 10], *cur = dat;
} subs;
struct node {
  unsigned int val;
  unsigned int lc = 0, rc = 0, siz = 0;
} nodes[maxn + 10];
bool exist[maxn + 10];
unsigned long long ans = 0;

unsigned int dfsSize(const unsigned int x)
{
  nodes[x].siz = 1;
  if (nodes[x].lc) nodes[x].siz += dfsSize(nodes[x].lc);
  if (nodes[x].rc) nodes[x].siz += dfsSize(nodes[x].rc);
  return nodes[x].siz;
}
inline void clear()
{
  for (auto i : subs)
    exist[i] = 0;
  subs.clear();
}
inline void add(const unsigned int x)
{
  subs.push_back(x);
  exist[x] = true;
}
void dfsCount(const unsigned int x, const unsigned int p)
{
  if (!x) return;
  if (nodes[x].val < p && exist[p - nodes[x].val]) ++ans;
  dfsCount(nodes[x].lc, p);
  dfsCount(nodes[x].rc, p);
}
void addSubTree(const unsigned int x)
{
  if (!x) return;
  add(nodes[x].val);
  addSubTree(nodes[x].lc);
  addSubTree(nodes[x].rc);
}
void dfs(const unsigned int x)
{
  if (!x) return;
  unsigned int ls = nodes[x].lc, rs = nodes[x].rc;
  if (nodes[ls].siz < nodes[rs].siz) swap(ls, rs);
  dfs(rs);
  clear();
  dfs(ls);
  dfsCount(rs, nodes[x].val);
  add(nodes[x].val);
  addSubTree(rs);
}
static unsigned int buildTree(const unsigned int n)
{
  static unsigned int stk[maxn + 1], *top = stk;
  *(top++) = 1;
  unsigned int root = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    unsigned int* ptr = top;
    while (ptr > stk && nodes[*(ptr - 1)].val < nodes[i].val)
      --ptr;
    if (ptr > stk)
      nodes[*(ptr - 1)].rc = i;
    else
      root = i;
    if (ptr < top) nodes[i].lc = *ptr;
    top = ptr;
    *(top++) = i;
  }
  return root;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i)
    cin >> nodes[i].val;
  const unsigned int rt = buildTree(n);
  dfsSize(rt);
  dfs(rt);
  cout << ans << endl;
  return 0;
}