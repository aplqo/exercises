#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 1e5, maxc = maxn;

struct edge {
  unsigned int to;
  const edge* pre;
} ed[maxn * 2];
const edge* head[maxn + 1];
unsigned int son[maxn + 1];
unsigned int c[maxn + 1];
unsigned long long ans[maxn + 1];
class Array {
 public:
  unsigned int* begin() { return val; }
  unsigned int* end() { return cur; }
  void push_back(const unsigned int v) { *(cur++) = v; }
  void clear() { cur = val; }

 private:
  unsigned int val[maxc + 1], *cur = val;
} col;
unsigned int cnt[maxc + 1], mval = 0;
unsigned long long sum[maxc + 1];

void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
unsigned int dfsSon(const unsigned int x, const unsigned int f)
{
  unsigned int ret = 1, ms = 0;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const unsigned int siz = dfsSon(to, x);
    ret += siz;
    if (siz > ms) {
      ms = siz;
      son[x] = to;
    }
  }
  return ret;
}
inline void addColor(const unsigned int x)
{
  if (!cnt[x])
    col.push_back(x);
  else
    sum[cnt[x]] -= x;
  ++cnt[x];
  sum[cnt[x]] += x;
  mval = max(mval, cnt[x]);
}
inline void clearColor()
{
  for (unsigned int j : col) {
    sum[cnt[j]] = 0;
    cnt[j] = 0;
  }
  col.clear();
  mval = 0;
}
void dfsCount(const unsigned int x, const unsigned int f)
{
  addColor(c[x]);
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != f) dfsCount(i->to, x);
}
void dfs(const unsigned int x, const unsigned int f)
{
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != f && i->to != son[x]) {
      dfs(i->to, x);
      clearColor();
    }
  if (son[x]) dfs(son[x], x);

  addColor(c[x]);
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != f && i->to != son[x]) dfsCount(i->to, x);
  ans[x] = sum[mval];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<unsigned int>(cin), n, c + 1);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addEdge(x, y);
    addEdge(y, x);
  }
  dfsSon(1, 0);
  dfs(1, 0);
  copy_n(ans + 1, n, ostream_iterator<unsigned long long>(cout, " "));
  cout << endl;
  return 0;
}