#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
using res_t = int;
constexpr res_t mod = 1e9 + 7;
const int maxn = 5000;

res_t ans[2][maxn + 10], sum[2][maxn + 10];

inline void getSum(const res_t i, const res_t beg, const res_t end)
{
  partial_sum(
      ans[i] + beg, ans[i] + end + 1, sum[i] + beg,
      [](const res_t a, const res_t b) -> res_t { return (a + b) % mod; });
}

inline void solveL(const res_t b, const res_t k)
{
  const res_t beg = 1, end = b;
  fill(ans[0] + beg, ans[0] + end + 1, 1);
  getSum(0, beg, end);
  for (res_t i = 1; i <= k; ++i) {
    const res_t pos = i & 0x01, lst = pos ^ 0x01;
    for (res_t j = beg; j < end; ++j) {
      ans[pos][j] =
          (sum[lst][b - 1] - (2 * j - b >= 1 ? sum[lst][2 * j - b] : 0) + mod) %
          mod;
      ans[pos][j] = (ans[pos][j] - ans[lst][j] + mod) % mod;
    }
    getSum(pos, beg, end);
  }
}
inline void solveG(const res_t n, const res_t b, const res_t k)
{
  const res_t beg = b + 1, end = n + 1;
  fill(ans[0] + beg, ans[0] + end + 1, 1);
  getSum(0, beg, end);
  for (res_t i = 1; i <= k; ++i) {
    const res_t pos = i & 0x01, lst = pos ^ 0x01;
    for (res_t j = beg; j < end; ++j)
      ans[pos][j] =
          ((2 * j - b - 1 > n ? sum[lst][n] : sum[lst][2 * j - b - 1]) -
           ans[lst][j] + mod) %
          mod;
    getSum(pos, beg, end);
  }
}
int main()
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  res_t n, a, b, k;
  cin >> n >> a >> b >> k;

  if (a > b)
    solveG(n, b, k);
  else
    solveL(b, k);
  cout << ans[k & 0x01][a] << endl;
  return 0;
}