#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
using namespace std;
using num_t = unsigned long long;
const int maxn = 100;

num_t mat[maxn + 10][maxn + 10];
bool negtive[maxn + 10][maxn + 10];
num_t rem[maxn + 10];
unsigned int vis[maxn + 10];

bool fun(const unsigned int cur, const unsigned int n)
{
  if (cur == n) return true;
  unsigned int tmp[maxn + 10];
  iota(tmp, tmp + n, 1);
  shuffle(tmp, tmp + n, mt19937(random_device()()));
  vis[cur] = true;
  for (unsigned int* i = tmp; i < tmp + n; ++i) {
    if (!mat[cur][*i]) continue;
    if (negtive[cur][*i] && rem[cur] * 2 <= mat[cur][*i]) continue;
    num_t r = negtive[cur][*i] ? (rem[cur] * 2 - mat[cur][*i])
                               : (rem[cur] * 2 + mat[cur][*i]);
    if (vis[*i] && r < rem[*i]) continue;
    rem[*i] = r;
    if (fun(*i, n)) return true;
  }
  return false;
}
int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, l = 0, r = 0;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= n; ++j) {
      long long t;
      cin >> t;
      if (t < 0) {
        r += -t;
        mat[i][j] = -t;
        negtive[i][j] = true;
      }
      else
        mat[i][j] = t;
    }
  if (r == 0) {
    cout << "1" << endl;
    return 0;
  }
  while (l <= r) {
    unsigned int m = (l + r) >> 1;
    fill(vis, vis + n + 1, false);
    rem[1] = m;
    if (fun(1, n))
      r = m - 1;
    else
      l = m + 1;
  }
  cout << l << endl;
  return 0;
}