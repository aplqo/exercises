#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdio>
#include <queue>
using namespace std;
typedef long long num_t;
const int maxn = 100, maxm = 2500, maxk = 50;

struct edge {
  unsigned int to;
  num_t val;
  edge* pre = nullptr;
} ed[maxm * 2 + 10];
edge* head[maxn + 10];
bool vis[maxn + 10][maxk + 10];
num_t ans[maxn + 10][maxk + 10];

inline void AddEdge(const unsigned int from, const unsigned int to,
                    const num_t val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void spfa(const unsigned int n, const unsigned int k)
{
  queue<unsigned int> q;
  q.push(0);
  ans[0][0] = 0;
  for (unsigned int i = 0; i <= k; ++i)
    fill(ans[i], ans[i] + k + 1, LLONG_MAX);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    const unsigned int po = cur % n, lay = cur / n;
    vis[po][lay] = false;
    for (edge* i = head[po]; i; i = i->pre) {
      if (ans[po][lay] + i->val < ans[i->to][lay]) {
        ans[i->to][lay] = ans[po][lay] + i->to;
        if (!vis[i->to][lay]) {
          q.push(lay * n + i->to);
          vis[i->to][lay] = true;
        }
      }
      if (lay < k && ans[po][lay] - i->val < ans[i->to][lay + 1]) {
        ans[i->to][lay + 1] = ans[po][lay] - i->to;
        if (!vis[i->to][lay + 1]) {
          q.push((lay + 1) * n + i->to);
          ans[i->to][lay + 1] = true;
        }
      }
    }
  }
}
int main()
{
#if (!defined(APTEST)) && (!defined(APDEBUG))
  freopen("magic.in", "r", stdin);
  freopen("magic.out", "w", stdout);
#endif
  unsigned int n, m, k;
  scanf("%u%u%u", &n, &m, &k);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    num_t c;
    scanf("%u%u%lld", &u, &v, &c);
    AddEdge(u - 1, v - 1, c);
  }
  spfa(n, k);
  printf("%lld\n", ans[n - 1][k]);
  return 0;
}