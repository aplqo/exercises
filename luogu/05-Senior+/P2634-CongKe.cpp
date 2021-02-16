#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 2e4;

struct edge {
  unsigned int to;
  unsigned int val;
  const edge* pre;
} ed[maxn * 2];
const edge* head[maxn + 1];
unsigned int ans[3], cnt[3], sub[3];
unsigned int siz[maxn + 1];
bool vis[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to,
             const unsigned int val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void dfsPoint(const unsigned int x, const unsigned int f, const unsigned int n,
              unsigned int& pos, unsigned int& maxv)
{
  unsigned int ms = 0;
  siz[x] = 1;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to]) continue;
    dfsPoint(to, x, n, pos, maxv);
    siz[x] += siz[to];
    ms = max(ms, siz[to]);
  }
  ms = max(ms, n - siz[x]);
  if (ms < maxv) {
    pos = x;
    maxv = ms;
  }
}
inline unsigned int treeCenter(const unsigned int n, const unsigned int x)
{
  unsigned int ret, ms = UINT_MAX;
  dfsPoint(x, 0, n, ret, ms);
  return ret;
}
void dfsSubtree(const unsigned int x, const unsigned int f,
                const unsigned int dis)
{
  ++sub[dis];
  for (unsigned int i = 0; i < 3; ++i)
    ans[(i + dis) % 3] += 2 * cnt[i];
  for (const edge* i = head[x]; i; i = i->pre)
    if (i->to != f && !vis[i->to]) dfsSubtree(i->to, x, (dis + i->val) % 3);
}
void divide(const unsigned int x)
{
  vis[x] = true;
  cnt[0] = 1;
  ++ans[0];
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (vis[to]) continue;
    dfsSubtree(to, x, (i->val) % 3);
    for (unsigned int i = 0; i < 3; ++i) {
      cnt[i] += sub[i];
      sub[i] = 0;
    }
  }

  fill(cnt, cnt + 3, 0);
  for (const edge* i = head[x]; i; i = i->pre)
    if (!vis[i->to]) divide(treeCenter(siz[i->to], i->to));
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y, w;
    cin >> x >> y >> w;
    addEdge(x, y, w);
    addEdge(y, x, w);
  }
  divide(treeCenter(n, 1));
  {
    unsigned int s = ans[0], f = ans[0] + ans[1] + ans[2];
    const unsigned int g = gcd(s, f);
    s /= g;
    f /= g;
    cout << s << "/" << f << endl;
  }
  return 0;
}