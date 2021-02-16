#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <ctime>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
using namespace std;
const unsigned int maxn = 30;
constexpr unsigned int randBit = numeric_limits<double>::digits;
constexpr double InitTemp = 1e5, Ratio = 0.97, eps = 1e-3, maxTime = 0.5;

namespace dsu {
unsigned int siz[maxn * 2 + 1], cnt[maxn * 2 + 1], fa[maxn * 2 + 1];
static void init(const unsigned int n)
{
  fill(siz, siz + n * 2, 1);
  fill(cnt, cnt + n, 1);
  iota(fa, fa + n * 2, 0);
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
  cnt[x] += cnt[y];
}
}  // namespace dsu
unsigned int blk[maxn * 2 + 1], buf[maxn * 2 + 1];

static unsigned int read(const unsigned int n)
{
  unsigned int m = 0;
  for (unsigned int i = 0; i < n; ++i) {
    static char str[maxn + 1];
    cin >> str;
    for (unsigned int j = 0; j < n; ++j)
      if (str[j] == '1') {
        dsu::merge(i, j + n);
        ++m;
      }
  }
  return m;
}
static unsigned int getBlk(const unsigned int n)
{
  unsigned int pos = 0;
  for (unsigned int i = 0; i < n * 2; ++i)
    if (dsu::find(i) == i) blk[pos++] = i;
  return pos;
}
int getCost(const unsigned int seq[], const unsigned int n)
{
  unsigned int ret = 0;
  unsigned int cx = 0, cy = 0;
  for (const unsigned int* i = seq; i < seq + n; ++i) {
    const unsigned int x = dsu::cnt[*i], y = dsu::siz[*i] - dsu::cnt[*i];
    ret += x * cy + cx * y + x * y;
    cx += x;
    cy += y;
    if (cx == cy) cx = cy = 0;
  }
  return ret;
}
unsigned int simulateAnneal(const unsigned int n)
{
  static mt19937 rnd((random_device())());
  int ret = getCost(blk, n), cost = ret;
  copy(blk, blk + n, buf);
  for (double temp = InitTemp; temp > eps; temp *= Ratio) {
    shuffle(buf, buf + n, rnd);
    const int newCost = getCost(buf, n);
    ret = min(ret, newCost);
    if (newCost > cost && generate_canonical<double, randBit>(rnd) >
                              exp(-(newCost - cost) / temp))
      copy(blk, blk + n, buf);
    else
      copy(buf, buf + n, blk);
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  dsu::init(n);
  const unsigned int m = read(n);
  const unsigned int bk = getBlk(n);
  unsigned int ans = UINT_MAX;
  const clock_t beg = clock();
  while (1.0 * (clock() - beg) / CLOCKS_PER_SEC < maxTime)
    ans = min(ans, simulateAnneal(bk));
  cout << ans - m << "\n";
  return 0;
}