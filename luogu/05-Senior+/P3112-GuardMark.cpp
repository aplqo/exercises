#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 20, maxs = 1 << maxn;
constexpr unsigned long long inf = ULLONG_MAX;

unsigned int lg[maxs + 1];
unsigned long long h[maxn + 1], w[maxn + 1], lim[maxn + 1];
unsigned long long sumH[maxs + 1], sumW[maxs + 1];

#define lowbit(x) ((x) & -(x))
static void initLog(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    lg[1 << i] = i;
}
static void initSum(const unsigned int n)
{
  const unsigned int msk = 1 << n;
  for (unsigned int i = 1; i < msk; ++i) {
    const unsigned int lb = lowbit(i);
    sumH[i] = sumH[i - lb] + h[lg[lb]];
    sumW[i] = sumW[i - lb] + w[lg[lb]];
  }
}
pair<bool, unsigned long long> dp(const unsigned int n,
                                  const unsigned long long H)
{
  static unsigned long long f[maxs + 1] = {inf};
  static bool exist[maxs + 10] = {true};
  const unsigned int msk = (1 << n) - 1;
  for (unsigned int i = 0; i <= msk; ++i) {
    if (!exist[i]) continue;
    for (unsigned int j = (~i) & msk; j; j -= lowbit(j)) {
      const unsigned int lb = lowbit(j), p = lg[lb];
      if (sumW[i] <= lim[p]) {
        f[i | lb] = max(f[i | lb], min(f[i], lim[p] - sumW[i]));
        exist[i | lb] = true;
      }
    }
  }
  unsigned long long ret = 0;
  bool suc = false;
  for (unsigned int i = 0; i < msk; ++i)
    if (sumH[i] >= H && exist[i]) {
      ret = max(f[i], ret);
      suc = true;
    }
  return make_pair(suc, ret);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  unsigned long long H;
  cin >> n >> H;
  for (unsigned int i = 0; i < n; ++i)
    cin >> h[i] >> w[i] >> lim[i];
  initLog(n);
  initSum(n);
  const auto [suc, val] = dp(n, H);
  if (!suc)
    cout << "Mark is too tall";
  else
    cout << val;
  cout.put('\n');
  return 0;
}