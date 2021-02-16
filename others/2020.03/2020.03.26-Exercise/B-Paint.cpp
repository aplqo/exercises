#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
using num_t = unsigned int;
const int maxn = 30, maxm = 10;

struct vertex {
  unsigned int col = 0;
  bool mat[maxn + 10];
} ve[maxn + 10];
unsigned int cnt[maxm + 10], col[maxn + 10];
unsigned int m;

namespace find {
unsigned int fa[maxn + 10], siz[maxn + 10];
inline void Init(const unsigned int n)
{
  iota(fa, fa + n, 0);
  fill(siz, siz + n, 1);
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
  siz[fx] += siz[fy];
  fa[fy] = fx;
}
}  // namespace find
num_t dfs(const unsigned int x, const unsigned int n)
{
  if (x >= n) {
    for (unsigned int i = 0; i < n; ++i)
      if (!all_of(ve, ve + n, [&i](const vertex& j) {
            return !j.mat[i] || j.col != ve[i].col;
          }))
        return 0;
    return 1;
  }
  num_t ret = 0;
  for (unsigned int c = 1; c <= m; ++c) {
    if (!all_of(ve, ve + n,
                [&c, &x](const vertex& i) { return !i.mat[x] || i.col != c; }))
      continue;
    ve[x].col = c;
    ++cnt[c];
    ret += dfs(x + 1, n);
    ve[x].col = 0;
    --cnt[c];
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n >> m;
  find::Init(n);
  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = 0; j < n; ++j) {
      unsigned int t;
      cin >> t;
      ve[i].mat[j] = t;
      if (t) find::Merge(i, j);
    }
  num_t ans = 1;
  for (unsigned int i = 0; i < n; ++i)
    if (find::GetFather(i) == i) ans *= dfs(i, find::siz[i]);
  if (ans)
    cout << ans << endl;
  else
    cout << "Poor +Q" << endl;
  return 0;
}