#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 50, maxm = 50, maxt = 2500;

bool board[maxn + 1][maxm + 1];
unsigned int fb[maxn + 1][maxm + 1], f[maxn + 1][maxt + 1];
unsigned int cnt[maxm + 1][2];

static void dpBoard(const unsigned int p, const unsigned int m)
{
  static unsigned int f[maxm + 1][maxm + 1][2];
  for (unsigned int i = 1; i <= m; ++i)
    for (unsigned int t = 1; t <= i; ++t) {
      f[i][t][0] = max(f[i - 1][t][0], f[i - 1][t][1]);
      f[i][t][1] = f[i - 1][t - 1][0] + 1;
      for (unsigned int k = 1; k < i; ++k) {
        if (board[p][k] == board[p][i])
          f[i][t][1] = max(f[i][t][1], f[k][t][1] + cnt[i][board[p][i]] -
                                           cnt[k][board[p][i]]);
        else
          f[i][t][1] = max(f[i][t][1], f[k][t - 1][1] + cnt[i][board[p][i]] -
                                           cnt[k][board[p][i]]);
      }
    }
  for (unsigned int i = 1; i <= m; ++i)
    fb[p][i] = max(f[m][i][0], f[m][i][1]);
}
static void init(const unsigned int n, const unsigned int m)
{
  for (unsigned int i = 1; i <= n; ++i) {
    for (unsigned int j = 1; j <= m; ++j) {
      const bool t = board[i][j];
      cnt[j][t] = cnt[j - 1][t] + 1;
      cnt[j][!t] = cnt[j - 1][!t];
    }
    dpBoard(i, m);
  }
}
unsigned int dp(const unsigned int n, const unsigned int m,
                const unsigned int t)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = 1; j <= t; ++j)
      for (unsigned int k = j >= m ? j - m : 0; k <= j; ++k)
        f[i][j] = max(f[i][j], f[i - 1][k] + fb[i][j - k]);
  return f[n][t];
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, t;
  cin >> n >> m >> t;
  for (unsigned int i = 1; i <= n; ++i) {
    static char str[maxm + 1];
    cin >> str;
    for (unsigned int j = 0; j < m; ++j)
      board[i][j + 1] = str[j] == '1';
  }
  init(n, m);
  cout << dp(n, m, t) << endl;
  return 0;
}
