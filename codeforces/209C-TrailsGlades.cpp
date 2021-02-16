#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 1e6, maxm = 1e6;

unsigned int degree[maxn + 10], blk[maxn + 10];
namespace find {
unsigned int fa[maxn + 10], siz[maxn + 10];
inline void Init(const unsigned int n)
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
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  find::Init(n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    find::Merge(a, b);
    ++degree[a];
    ++degree[b];
  }

  unsigned long long ans = 0;
  const unsigned int r = find::GetFather(1);
  bool fl = true;
  for (unsigned int i = 1; i <= n; ++i) {
    if (degree[i] & 0x01) ++blk[find::GetFather(i)];
    if (degree[i] && find::GetFather(i) != r) fl = false;
  }
  if (fl && !blk[r]) {
    cout << "0" << endl;
    return 0;
  }
  for (unsigned int i = 1; i <= n; ++i)
    if (find::GetFather(i) == i) {
      ans += blk[i] / 2;
      if (degree[i] && !blk[i]) ++ans;
    }
  if (!degree[1] && find::siz[1] == 1 && find::GetFather(1) == 1) ++ans;
  cout << ans << endl;
  return 0;
}