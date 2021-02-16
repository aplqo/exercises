#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <numeric>
#include <queue>
using namespace std;
typedef int num;
const num inf = INT_MAX, ninf = INT_MIN / 2;
const int maxn = 110, maxm = 1010 + maxn;

struct edge {
  edge() { pre = NULL; }
  int to;
  num val;
  edge* pre;
} ed[maxm * 2 + maxn], *cur = ed;
edge* head[maxn];
int dep[maxn];
num ans[maxn];
bool vis[maxn];
int n = maxn, m;

inline void AddEdge(int from, int to, num val)
{
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline bool spfa(int s)
{
  queue<int> q;
  q.push(s);
  vis[s] = true;
  dep[s] = 1;
  ans[s] = 0;
  while (!q.empty()) {
    int cur = q.front();
    q.pop();
    vis[cur] = false;
    for (edge* i = head[cur]; i; i = i->pre)
      if (ans[i->to] < ans[cur] + i->val) {
        ans[i->to] = ans[cur] + i->val;
        ++dep[i->to];
        if (dep[i->to] > n) return false;
        if (!vis[i->to]) {
          vis[i->to] = true;
          q.push(i->to);
        }
      }
  }
  return true;
}
inline void solve()
{
  cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int s, t;
    num v;
    cin >> s >> t >> v;
    AddEdge(s - 1, t, v);
    AddEdge(t, s - 1, -v);
  }
  for (int i = 0; i < n; ++i)
    AddEdge(i, i + 1, ninf);
  cout << boolalpha << spfa(0) << endl;
}
int main()
{
  ios_base::sync_with_stdio(false);
  int w;
  cin >> w;
  for (int i = 0; i < w; ++i) {
    fill(ans, ans + n + 1, ninf);
    solve();
    fill(dep, dep + n + 1, 0);
    fill(vis, vis + n + 1, false);
    fill(head, head + n + 1, static_cast<edge*>(NULL));
    cur = ed;
  }
  return 0;
}