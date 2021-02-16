#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e5;

struct edge {
  unsigned int to;
  long long val;
  edge* pre;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
int siz[maxn + 1], degree[maxn + 1];
long long sum[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to,
             const long long val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
  ++degree[from];
}
long long dfsSize(const unsigned int x, const unsigned int f)
{
  long long ret = 0;
  for (edge* i = head[x]; i; i = i->pre) {
    if (i->to == f) continue;
    ret += dfsSize(i->to, x) + i->val * siz[i->to];
    siz[x] += siz[i->to];
  }
  ++siz[x];
  return ret;
}
inline long long change(const unsigned int from, const unsigned int to,
                        long long ans, const long long l)
{
  ans -= l * siz[to];
  siz[from] -= siz[to];
  siz[to] += siz[from];
  ans += l * siz[from];
  return ans;
}
void dfsAns(const unsigned int x, const unsigned int f, const long long ans)
{
  if (degree[x] == 1) sum[x] = ans;
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    dfsAns(to, x, change(x, to, ans, i->val));
    change(to, x, ans, i->val);
  }
}
int main()
{
  unsigned int n;
  scanf("%u", &n);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v;
    int w;
    scanf("%d%u%u", &w, &u, &v);
    addEdge(u, v, w);
    addEdge(v, u, w);
  }
  dfsAns(1, 0, dfsSize(1, 0));
  printf("%lld\n", accumulate(sum + 1, sum + 1 + n, 0ll));
  return 0;
}
