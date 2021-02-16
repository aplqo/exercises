#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <iterator>
#include <limits>
using namespace std;
typedef int_fast32_t num;
const int maxn = 300000, maxl = 20;
const num inf = numeric_limits<num>::max();

struct edge {
  int to;
  num val;
  edge *pre, *nxt;
} ed[(maxn + 10) * 2], *cur = ed;
edge* head[maxn + 10];
int fa[maxn + 10][maxl + 10], dep[maxn + 10], lg[maxn + 10];
num dis[maxn + 10];
struct plan {
  int from, to, lca;
  num val;
} pl[maxn + 10];
num maxval, minval;
int fla[maxn + 10];
int n, m;

template <class T>
inline void read(T& dat)
{
  dat = 0;
  char c = getchar();
  while (!isdigit(c))
    c = getchar();
  while (isdigit(c)) {
    dat = dat * 10 + c - '0';
    c = getchar();
  }
}
inline void AddEdge(int from, int to, num val)
{
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  if (head[from]) head[from]->nxt = cur;
  head[from] = cur;
  ++cur;
}
/*-----lca-----*/
inline void getLog(int n)
{
  for (int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void init(int x, int f)
{
  dep[x] = dep[f] + 1;
  fa[x][0] = f;
  for (int i = 1; i <= lg[dep[x]]; ++i)
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
  for (edge* i = head[x]; i; i = i->pre)
    if (!dep[i->to]) {
      dis[i->to] = dis[x] + i->val;
      init(i->to, x);
    }
    else {
      if (i->pre) i->pre->nxt = i->nxt;
      if (i->nxt)
        i->nxt->pre = i->pre;
      else
        head[x] = i->pre;
    }
}
inline int lca(int a, int b)
{
  if (dep[a] != dep[b]) {
    if (dep[a] > dep[b]) swap(a, b);
    int x = dep[b] - dep[a], t = 1;
    for (int i = 0; x; ++i, t <<= 1)
      if (x & t) {
        b = fa[b][i];
        x ^= t;
      }
  }
  if (a == b) return a;
  for (int i = lg[dep[a]] + 1; i;) {
    int k = i - 1;
    if (fa[a][k] == fa[b][k])
      --i;
    else {
      a = fa[a][k];
      b = fa[b][k];
    }
  }
  return fa[a][0];
}

void dfs(int x)
{
  for (edge* i = head[x]; i; i = i->pre) {
    dfs(i->to);
    fla[x] += fla[i->to];
  }
}
inline bool test(num d)
{
  static num lstd = inf;
  static plan* p = pl;
  const auto cmp = [&d](const plan& a) { return a.val > d; };
  if (d > lstd)
    p = partition(pl, p, cmp);
  else
    p = partition(p, pl + m, cmp);
  lstd = d;
  fill(fla, fla + n + 1, 0);
  for (const plan* i = pl; i < p; ++i) {
    ++fla[i->from];
    ++fla[i->to];
    fla[i->lca] -= 2;
  }
  dfs(1);
  for (int i = 1; i <= n; ++i)
    for (edge* j = head[i]; j; j = j->pre)
      if (fla[j->to] == p - pl && j->val >= maxval - lstd) return true;
  return false;
}

int main()
{
  read(n);
  read(m);
  getLog(n);
  for (int i = 1; i < n; ++i) {
    int a, b;
    num t;
    read(a);
    read(b);
    read(t);
    AddEdge(a, b, t);
    AddEdge(b, a, t);
  }
  init(1, 0);
  for (plan* i = pl; i < pl + m; ++i) {
    read(i->from);
    read(i->to);
  }
  for (plan* i = pl; i < pl + m; ++i) {
    i->lca = lca(i->from, i->to);
    i->val = dis[i->from] + dis[i->to] - 2 * dis[i->lca];
  }
  maxval = max_element(pl, pl + m, [](const plan& a, const plan& b) {
             return a.val < b.val;
           })->val;
  minval =
      maxval - max_element(ed, cur, [](const edge& a, const edge& b) -> bool {
                 return a.val < b.val;
               })->val;

  num l = minval, r = maxval;
  while (l <= r) {
    num m = (l + r) >> 1;
    if (test(m))
      r = m - 1;
    else
      l = m + 1;
  }
  cout << l << endl;
  return 0;
}