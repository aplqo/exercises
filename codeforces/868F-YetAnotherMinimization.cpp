#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 1e5, maxk = 20;
constexpr unsigned long long inf = ULLONG_MAX / 2;

struct Range {
  unsigned int pl, pr;
  unsigned int ansl, ansr;
};
unsigned int a[maxn + 10];
unsigned long long f[maxk + 10][maxn + 10], cost;
unsigned int cnt[maxn + 10];

inline void add(const unsigned int v)
{
  cost += cnt[v];
  ++cnt[v];
}
inline void remove(const unsigned int v)
{
  --cnt[v];
  cost -= cnt[v];
}
inline void extract(const unsigned int* const ql, const unsigned int* const qr)
{
  static const unsigned int *pl = a + 1, *pr = a + 1;
  while (pl > ql)
    add(*(--pl));
  while (pr < qr)
    add(*(pr++));
  while (pl < ql)
    remove(*(pl++));
  while (pr > qr)
    remove(*(--pr));
}
static void init(const unsigned int n, const unsigned int k)
{
  for (unsigned int i = 0; i <= k; ++i)
    fill(f[i], f[i] + n + 1, inf);
  f[0][0] = 0;
  for (unsigned int i = 1; i <= n; ++i) {
    add(a[i]);
    f[1][i] = cost;
  }
  memset(cnt, 0, sizeof(cnt));
  cost = 0;
}
void conquer(const unsigned int k, const Range ini)
{
  queue<Range> q;
  q.push(ini);
  while (!q.empty()) {
    const Range cur = q.front();
    q.pop();
    if (cur.pl >= cur.pr) continue;
    const unsigned int mid = (cur.pl + cur.pr) >> 1,
                       mp = min(mid - 1, cur.ansr);
    const unsigned int* const epos = a + mid + 1;
    unsigned int pos = 0;
    extract(a + cur.ansl, epos);
    for (unsigned int i = cur.ansl; i <= mp; ++i) {
      extract(a + i + 1, epos);
      if (f[k - 1][i] + cost <= f[k][mid]) {
        f[k][mid] = f[k - 1][i] + cost;
        pos = i;
      }
    }
    q.push(Range{cur.pl, mid, cur.ansl, pos});
    q.push(Range{mid + 1, cur.pr, pos, cur.ansr});
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
  init(n, k);
  for (unsigned int i = 2; i <= k; ++i)
    conquer(i, Range{i, n + 1, i - 1, n});
  cout << f[k][n] << "\n";
  return 0;
}