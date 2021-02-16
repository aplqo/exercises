#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1000000;

struct edge {
  unsigned int to, from;
  bool color;
} ed[maxn + 10];
unsigned int degree[maxn + 10];
unsigned int fail[maxn + 10], black[maxn + 10];
unsigned int failblock;
unsigned int ans;

namespace find {
static unsigned int fa[maxn + 10], siz[maxn + 10];
inline void init(const unsigned int n)
{
  iota(fa, fa + n + 1, 0);
  fill(siz, siz + n + 1, 1);
}
unsigned int GetFather(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = GetFather(fa[x]);
}
inline void Merge(const unsigned int x, const unsigned int y)
{
  unsigned int fx = GetFather(x), fy = GetFather(y);
  if (fx == fy) return;
  if (siz[fx] < siz[fy]) swap(fx, fy);
  fa[fy] = fx;
  siz[fx] += siz[fy];
}
}  // namespace find
inline void init(const unsigned int n, const unsigned int m)
{
  for (const edge* i = ed; i < ed + m; ++i) {
    const unsigned int f = find::GetFather(i->from);
    if (i->color) {
      ++black[f];
      ++degree[i->from];
      ++degree[i->to];
    }
    else {
      degree[i->from] += 2;
      degree[i->to] += 2;
    }
  }
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int f = find::GetFather(i);
    if (degree[i] & 0x01) ++fail[f];
  }
  {
    static const auto f = [](unsigned int i) -> bool { return i; };
    ans = count_if(black, black + n + 1, f);
    failblock = count_if(fail, fail + n + 1, f);
  }
}
inline void Inverse(edge& p)
{
  const unsigned int f = find::GetFather(p.from);
  const auto cond = [](unsigned int& dat, bool c1, bool c2 = true) {
    if (c1)
      ++dat;
    else if (c2)
      --dat;
  };
  cond(failblock, false, fail[f] && black[f]);
  cond(fail[f], false, degree[p.from] & 0x01);
  cond(fail[f], false, degree[p.to] & 0x01);
  if (p.color) {
    p.color = false;
    --black[f];
    cond(ans, false, !black[f]);
    ++degree[p.from];
    ++degree[p.to];
  }
  else {
    p.color = true;
    cond(ans, !black[f], false);
    ++black[f];
    --degree[p.from];
    --degree[p.to];
  }
  cond(fail[f], degree[p.from] & 0x01, false);
  cond(fail[f], degree[p.to] & 0x01, false);
  cond(failblock, fail[f] && black[f], false);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, p;
  cin >> n >> m;
  find::init(n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int col;
    cin >> ed[i].from >> ed[i].to >> col;
    ed[i].color = col;
    find::Merge(ed[i].from, ed[i].to);
  }
  init(n, m);
  cin >> p;
  for (unsigned int i = 0; i < p; ++i) {
    unsigned int op, x;
    cin >> op;
    if (op == 1) {
      cin >> x;
      Inverse(ed[x]);
    }
    else {
      if (failblock)
        cout << "-1" << endl;
      else
        cout << ans << endl;
    }
  }
  return 0;
}