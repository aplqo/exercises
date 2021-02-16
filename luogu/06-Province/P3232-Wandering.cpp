#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <valarray>
#include <vector>
using namespace std;
const unsigned int maxn = 500, maxm = maxn * maxn;
using real = double;
constexpr real eps = 1e-10;

struct Edge {
  unsigned int from, to;
} ed[maxm + 1];
unsigned int degree[maxn + 1];
vector<unsigned int> gra[maxn + 1];
real equ[maxn + 1][maxn + 1];
real f[maxn + 1];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  static unsigned int id = 0;
  ed[id].from = from;
  ed[id++].to = to;
  ++degree[from];
  ++degree[to];
  gra[from].push_back(to);
  gra[to].push_back(from);
}
static void buildEquation(const unsigned int n)
{
  for (unsigned int i = 0; i < n - 1; ++i) {
    equ[i][i] = 1;
    for (unsigned int j : gra[i])
      if (j != n - 1) equ[i][j] = -real(1.0) / degree[j];
  }
  equ[0][n - 1] = 1;
}
static void gaussian(const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i) {
    if (fabs(equ[i][i]) <= eps) {
      for (unsigned int j = i + 1; j < n; ++j)
        if (fabs(equ[j][i]) > eps) {
          swap_ranges(equ[i], equ[i] + n + 1, equ[j]);
          break;
        }
    }
    for (unsigned int j = i + 1; j < n; ++j) {
      if (fabs(equ[j][i]) <= eps) continue;
      const real rat = equ[j][i] / equ[i][i];
      for (unsigned int k = i; k <= n; ++k)
        equ[j][k] -= rat * equ[i][k];
    }
  }
  for (unsigned int i = n - 1; i > 0; --i) {
    equ[i][n] /= equ[i][i];
    for (unsigned int j = 0; j < i; ++j)
      equ[j][n] -= equ[j][i] * equ[i][n];
  }
  equ[0][n] /= equ[0][0];
  for (unsigned int i = 0; i < n; ++i)
    f[i] = equ[i][n];
}
real sum(const unsigned int m)
{
  static real ex[maxm + 1];
  transform(ed, ed + m, ex, [](const Edge i) {
    return f[i.from] / degree[i.from] + f[i.to] / degree[i.to];
  });
  sort(ex, ex + m, greater<real>());
  real ret = 0;
  for (unsigned int i = 0; i < m; ++i)
    ret += ex[i] * (i + 1);
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u - 1, v - 1);
  }
  buildEquation(n);
  gaussian(n - 1);
  ios_base::sync_with_stdio(true);
  printf("%.3lf\n", sum(m));
  return 0;
}