#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 1e5, maxm = 1e5, maxc = 1e5;

char inbuf[maxn * 20 + maxm * 15 + 1000],
    *inpos = inbuf, outbuf[maxm * 10 + 1000], *outpos = outbuf;
template <class T = unsigned int>
inline T read()
{
  T ret = 0;
  char c = *(inpos++);
  while (!isdigit(c))
    c = *(inpos++);
  while (isdigit(c)) {
    ret = ret * 10 + c - '0';
    c = *(inpos++);
  }
  return ret;
}
inline void write(unsigned int x) { outpos += sprintf(outpos, "%u\n", x); }
struct edge {
  unsigned int to;
  const edge* pre;
} ed[maxn * 2 + 1];
const edge* head[maxn + 1];
struct Query {
  unsigned int k;
  unsigned int ans;
} qry[maxm + 1];
vector<Query*> seq[maxn + 1];
class Array {
 public:
  inline void push_back(const unsigned int v) { *(pos++) = v; }
  unsigned int* begin() { return val; }
  unsigned int* end() { return pos; }
  inline void clear() { pos = val; }

 private:
  unsigned int val[maxn + 1], *pos = val;
} sub;
unsigned int cnt[maxc + 1], n, siz[maxn + 1];
unsigned int son[maxn + 1], col[maxn + 1];

namespace ftree {
#define lowbit(x) ((x) & -(x))
int val[maxn + 1];
void modify(unsigned int p, const unsigned int n, const int delt)
{
  for (; p <= n; p += lowbit(p))
    val[p] += delt;
}
unsigned int query(unsigned int p)
{
  unsigned int ret = 0;
  for (; p; p -= lowbit(p))
    ret += val[p];
  return ret;
}

#undef lowbit
};  // namespace ftree
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
    const unsigned int sz = dfsSon(to, x);
    ret += sz;
    if (sz > ms) {
      ms = sz;
      son[x] = to;
    }
  }
  return siz[x] = ret;
}
inline void addColor(const unsigned int c)
{
  if (!cnt[c])
    sub.push_back(c);
  else
    ftree::modify(cnt[c], n, -1);
  ++cnt[c];
  ftree::modify(cnt[c], n, 1);
}
inline void clearColor()
{
  for (unsigned int i : sub) {
    ftree::modify(cnt[i], n, -1);
    cnt[i] = 0;
  }
  sub.clear();
}
void dfsCount(const unsigned int x, const unsigned int f)
{
  addColor(col[x]);
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != f) dfsCount(i->to, x);
}
void dfs(const unsigned int x, const unsigned int f)
{
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || son[x] == to) continue;
    dfs(to, x);
    clearColor();
  }
  if (son[x]) dfs(son[x], x);

  addColor(col[x]);
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != son[x] && i->to != f) dfsCount(i->to, x);
  const unsigned int ctyp = sub.end() - sub.begin();
  for (Query* i : seq[x])
    i->ans = i->k <= siz[x] ? ctyp - ftree::query(i->k - 1) : 0;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin.read(inbuf, sizeof(inbuf));
  n = read();
  const unsigned int m = read();
  generate_n(col + 1, n, read<unsigned int>);
  for (unsigned int i = 1; i < n; ++i) {
    const unsigned int a = read(), b = read();
    addEdge(a, b);
    addEdge(b, a);
  }
  for (Query* i = qry; i < qry + m; ++i) {
    unsigned int v = read();
    i->k = read();
    seq[v].push_back(i);
  }
  dfsSon(1, 0);
  dfs(1, 0);
  for_each(qry, qry + m, [](const Query& a) { write(a.ans); });
  cout.write(outbuf, outpos - outbuf);
  return 0;
}