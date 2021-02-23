#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <queue>
const int maxn = 1000, maxm = 3000, maxp = 10, maxc = 10;
const int maxMsk = 1u << maxp;
const int inf = INT_MAX / 2;

struct Edge {
  int to, val;
  const Edge* pre;
} ed[maxm * 2 + 1];
const Edge* head[maxn + 10];
int dis[maxMsk][maxn + 10];
bool vis[maxMsk][maxn + 10];
unsigned int key[maxn + 10], station[maxc + 10];

inline void addEdge(const int from, const int to, const int cost)
{
  static Edge* cur = ed;
  new (cur) Edge{to, cost, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, cost, head[to]};
  head[to] = cur++;
}
void dijkstra(const int n, const unsigned int msk)
{
  typedef std::pair<int, int> Pair;
  std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> q;
  int* const cdis = dis[msk];
  bool* const cvis = vis[msk];
  for (int i = 0; i < n; ++i)
    if (cdis[i] != inf) q.emplace(cdis[i], i);
  while (!q.empty()) {
    const Pair cur = q.top();
    q.pop();
    if (cvis[cur.second]) continue;
    cvis[cur.second] = true;
    for (const Edge* i = head[cur.second]; i; i = i->pre)
      if (cdis[i->to] > cur.first + i->val) {
        cdis[i->to] = cur.first + i->val;
        q.emplace(cdis[i->to], i->to);
      }
  }
}
void minSteinetTree(const int n, const unsigned int msk)
{
  for (unsigned int i = 1; i < msk; ++i)
    std::fill(dis[i], dis[i] + n, inf);
  for (int i = 0; i < n; ++i)
    dis[key[i]][i] = dis[0][i] = 0;
  for (unsigned int i = 1; i < msk; ++i) {
    for (unsigned int k = i; k; k = (k - 1) & i)
      for (int j = 0; j < n; ++j)
        dis[i][j] = std::min(dis[i][j], dis[k][j] + dis[i ^ k][j]);
    dijkstra(n, i);
  }
}
static int dp(const int n, const int c, const int p)
{
  static unsigned int validMsk[maxMsk], *end = validMsk;
  static bool vis[maxMsk];
  static int ans[maxMsk];
  const unsigned int msk = 1u << p;
  vis[0] = true;
  *(end++) = 0;
  for (unsigned int i = 0; i < msk; ++i)
    if (vis[i]) {
      for (const unsigned int* j = station; j < station + c; ++j) {
        const unsigned int nxt = i | *j;
        if (!vis[nxt]) {
          vis[nxt] = true;
          *(end++) = nxt;
        }
      }
      ans[i] = *std::min_element(dis[i], dis[i] + n);
    }
  for (unsigned int i = 0; i < msk; ++i)
    if (vis[i])
      for (const unsigned int* j = validMsk; j < end; ++j)
        ans[i | *j] = std::min(ans[i | *j], ans[i] + ans[*j]);
  return ans[msk - 1];
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m, p;
  std::cin >> n >> m >> p;
  for (int i = m; i; --i) {
    int u, v, w;
    std::cin >> u >> v >> w;
    addEdge(u - 1, v - 1, w);
  }
  int mxc = 0;
  for (int i = p; i; --i) {
    int id, freq;
    std::cin >> freq >> id;
    key[id - 1] = 1u << (i - 1);
    station[freq - 1] |= key[id - 1];
    mxc = std::max(mxc, freq);
  }
  minSteinetTree(n, 1u << p);
  std::cout << dp(n, mxc, p) << "\n";
  return 0;
}
