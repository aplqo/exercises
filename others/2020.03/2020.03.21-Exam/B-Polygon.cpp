#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
using namespace std;
using num_t = long long;
constexpr num_t ninf = numeric_limits<num_t>::min(),
                inf = numeric_limits<num_t>::max();
const int maxn = 50;

enum { MAX = 0, MIN = 1 };
struct vertex {
  num_t val;
  bool typ;  // true: mul false: add
} v[maxn * 2 + 10];
num_t f[2][maxn * 2 + 10][maxn * 2 + 10];

inline void dp(const unsigned int n)
{
  copy(v, v + n, v + n);
  for (unsigned int i = 0; i < n * 2; ++i) {
    fill(f[MAX][i], f[MAX][i] + n * 2, ninf);
    fill(f[MIN][i], f[MIN][i] + n * 2, inf);
  }
  for (unsigned int i = 0; i < n * 2; ++i)
    f[MAX][i][i] = f[MIN][i][i] = v[i].val;

  for (unsigned int d = 1; d < n * 2; ++d)
    for (unsigned int l = 0; l <= n * 2 - d; ++l) {
      const unsigned int r = l + d;
      for (unsigned int k = l; k < r; ++k) {
        if (v[k + 1].typ) {
          const num_t cma = f[MAX][l][k] * f[MAX][k + 1][r],
                      cmi = f[MIN][l][k] * f[MIN][k + 1][r];
          f[MAX][l][r] = max({f[MAX][l][r], cma, cmi});
          f[MIN][l][r] = min({f[MIN][l][r], cma, cmi});
        }
        else {
          f[MAX][l][r] = max(f[MAX][l][r], f[MAX][l][k] + f[MAX][k + 1][r]);
          f[MIN][l][r] = min(f[MIN][l][r], f[MIN][l][k] + f[MIN][k + 1][r]);
        }
      }
    }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  num_t ans = ninf;
  cin >> n;
  for (unsigned int i = 0; i < n; ++i) {
    char typ;
    cin >> typ >> v[i].val;
    v[i].typ = (typ == 'x');
  }
  dp(n);
  for (unsigned int i = 0; i < n; ++i)
    ans = max(ans, f[MAX][i][i + n - 1]);
  cout << ans << endl;
  return 0;
}