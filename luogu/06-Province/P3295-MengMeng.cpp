#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5, maxl = 16, maxv = maxn * maxl;
constexpr unsigned long long mod = 1e9 + 7;

namespace Disjoint {
unsigned int fa[maxv + 10], siz[maxv + 10];
static void init(const unsigned int n)
{
  iota(fa, fa + 1 + n, 0);
  fill(siz, siz + 1 + n, 1);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
}
}  // namespace Disjoint
unsigned int tab[maxn + 10][maxl + 10], vcnt, lg[maxn + 10], chl[maxv][2];

static void getLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
static void createTeble(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    tab[i][0] = ++vcnt;
  for (unsigned int i = 1; i <= lg[n]; ++i)
    for (unsigned int j = 1; j + (1 << i) <= n + 1; ++j) {
      tab[j][i] = ++vcnt;
      chl[vcnt][0] = tab[j][i - 1];
      chl[vcnt][1] = tab[j + (1 << (i - 1))][i - 1];
    }
}
static void init(const unsigned int n)
{
  getLog(n);
  createTeble(n);
  Disjoint::init(vcnt);
}
void link(const unsigned int l1, const unsigned int r1, const unsigned int l2,
          const unsigned int r2)
{
  const unsigned int d = lg[r2 - l2 + 1];
  Disjoint::merge(tab[l1][d], tab[l2][d]);
  Disjoint::merge(tab[r1 + 1 - (1 << d)][d], tab[r2 + 1 - (1 << d)][d]);
}

unsigned long long quickPow(unsigned long long a, unsigned int e)
{
  unsigned long long ret = 1;
  for (; e; e >>= 1) {
    if (e & 0x01) ret = (ret * a) % mod;
    a = (a * a) % mod;
  }
  return ret;
}
unsigned long long count(const unsigned int n)
{
  using namespace Disjoint;
  for (unsigned int i = lg[n]; i; --i) {
    const unsigned int mid = i - 1, off = 1 << mid;
    for (unsigned int j = 1; j + (1 << i) <= n + 1; ++j) {
      const unsigned rt = find(tab[j][i]);
      merge(chl[rt][0], tab[j][mid]);
      merge(chl[rt][1], tab[j + off][mid]);
    }
  }
  unsigned int cnt = 0;
  for (unsigned int i = 1; i <= n; ++i)
    if (find(i) == i) ++cnt;
  return (9 * quickPow(10, cnt - 1)) % mod;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  init(n);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int l1, r1, l2, r2;
    cin >> l1 >> r1 >> l2 >> r2;
    link(l1, r1, l2, r2);
  }
  cout << count(n) << "\n";
  return 0;
}