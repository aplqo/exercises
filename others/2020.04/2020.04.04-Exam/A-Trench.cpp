/*
 * Luogu team T127819: Weak Trench
 */
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#define lowbit(x) ((x) & -(x))
using namespace std;
const int maxx = 32768, maxn = 1e6;

struct point {
  unsigned int x, y;
} p[maxn + 10];
unsigned int ans[maxn + 10];
unsigned int tree[maxx + 10];

inline void Modify(unsigned int pos)
{
  for (; pos <= maxx; pos += lowbit(pos))
    ++tree[pos];
}
inline unsigned int Query(unsigned int pos)
{
  unsigned int ret = 0;
  for (; pos; pos -= lowbit(pos))
    ret += tree[pos];
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (point* i = p; i < p + n; ++i)
    cin >> i->x >> i->y;
  sort(p, p + n, [](const point a, const point b) {
    return a.x != b.x ? a.x < b.x : a.y < b.y;
  });
  for (point* i = p; i < p + n; ++i) {
    ++ans[Query(i->y)];
    Modify(i->y);
  }
  for (unsigned int i = 0; i <= n - 1; ++i)
    cout << ans[i] << endl;
  return 0;
}