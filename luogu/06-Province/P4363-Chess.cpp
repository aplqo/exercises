#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <numeric>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 10;
#define inc(x) (uint_fast64_t(1) << (x)*4)

int a[maxn + 1][maxn + 1], b[maxn + 1][maxn + 1];
unsigned int n, m;
uint_fast64_t en;

inline void toNum(uint_fast64_t stat, unsigned int val[])
{
  for (unsigned int i = 0; i < n; ++i) {
    val[i] = stat & 0x0f;
    stat >>= 4;
  }
}
int dfs(const uint_fast64_t stat)
{
  static unordered_map<uint_fast64_t, int> mem;
  if (stat == en) return 0;
  {
    auto it = mem.find(stat);
    if (it != mem.end()) return it->second;
  }
  unsigned int val[maxn];
  toNum(stat, val);

  const bool tur = !(accumulate(val, val + n, 0) & 0x01);
  const auto calc = [&val, tur, stat](const unsigned int p) {
    return dfs(stat + inc(p)) + (tur ? a[p][val[p] + 1] : -b[p][val[p] + 1]);
  };

  int ret;
  if (val[0] + 1 <= m)
    ret = calc(0);
  else
    ret = tur ? INT_MIN : INT_MAX;

  for (unsigned int i = 1; i < n; ++i)
    if (val[i] < val[i - 1]) ret = tur ? max(ret, calc(i)) : min(ret, calc(i));
  return mem[stat] = ret;
}
inline void read(const unsigned int n, const unsigned int m,
                 int dest[][maxn + 1])
{
  for (unsigned int i = 0; i < n; ++i)
    copy_n(istream_iterator<int>(cin), m, dest[i] + 1);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n >> m;
  read(n, m, a);
  read(n, m, b);
  for (unsigned int i = 0; i < n; ++i)
    en |= uint_fast64_t(m) << i * 4;
  cout << dfs(0) << endl;
  return 0;
}