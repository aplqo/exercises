#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <map>
using namespace std;
const unsigned int maxn = 300, maxv = 300, maxm = 1e5;
constexpr unsigned int inf = UINT_MAX / 2;

struct Store {
  unsigned int t, v;
  unsigned int c;
} st[maxn + 10];
struct Query {
  unsigned int m;
  unsigned int ans, t;
} qry[maxm + 1];
unsigned int f[maxn + 10][maxn * maxv + 10];

static void dp(const unsigned int n)
{
  for (unsigned int i = 0; i <= n; ++i)
    fill(f[i] + 1, f[i] + 1 + maxv * maxn + 2, inf);
  for (unsigned int i = 1; i <= n; ++i) {
    copy(f[i - 1], f[i - 1] + (i - 1) * maxn, f[i]);
    for (unsigned int j = st[i].v; j <= i * maxv + 1; ++j)
      f[i][j] = min(f[i][j], f[i - 1][j - st[i].v] + st[i].c);
    for (unsigned int j = i * maxv; j > 0; --j)
      f[i][j] = min(f[i][j], f[i][j + 1]);
  }
}
static void answerQuery(const unsigned int n, const unsigned int m)
{
  static Query* seq[maxm + 1];
  transform(qry, qry + m, seq, [](Query& i) { return &i; });
  sort(seq, seq + m, [](Query* a, Query* b) { return a->t < b->t; });
  unsigned int pos = 0;
  for (unsigned int i = 0; i < m; ++i) {
    while (pos + 1 <= n && st[pos + 1].t <= seq[i]->t)
      ++pos;
    seq[i]->ans =
        upper_bound(f[pos], f[pos] + n * maxv + 2, seq[i]->m) - f[pos] - 1;
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (Store* i = st + 1; i < st + 1 + n; ++i)
    cin >> i->c >> i->v >> i->t;
  for (Query* i = qry; i < qry + m; ++i)
    cin >> i->t >> i->m;
  sort(st + 1, st + 1 + n,
       [](const Store& a, const Store& b) { return a.t < b.t; });
  dp(n);
  answerQuery(n, m);
  for (Query* i = qry; i < qry + m; ++i)
    cout << i->ans << "\n";
  return 0;
}