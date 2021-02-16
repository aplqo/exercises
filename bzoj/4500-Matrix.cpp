#ifdef APDEBUG
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
typedef int num;
const int maxn = 1010;
const num inf = numeric_limits<num>::max();

struct edge {
  int to;
  num val;
  edge* pre = nullptr;
} ed[maxn * 4 + 1], *cur = ed;
edge* head[maxn * 2 + 1];
int dep[maxn * 2 + 1], n = maxn * 2, m;
num ans[maxn * 2 + 1];
bool vis[maxn * 2 + 1];

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
  ans[s] = 0;
  dep[s] = 1;
  while (!q.empty()) {
    int cur = q.front();
    q.pop();
    vis[cur] = false;
    for (edge* i = head[cur]; i; i = i->pre)
      if (ans[i->to] > ans[cur] + i->val) {
        ans[i->to] = ans[cur] + i->val;
        ++dep[i->to];
        if (dep[i->to] > n + m) return false;
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
  int k;
  cin >> n >> m >> k;
  for (int i = 0; i < k; ++i) {
    int a, b;
    num c;
    cin >> a >> b >> c;
    AddEdge(b + n, a, c);
    AddEdge(a, b + n, -c);
  }
  for (int i = 1; i <= n + m; ++i)
    AddEdge(0, i, inf / 2);
  cout << (spfa(0) ? "Yes" : "No") << endl;
}
int main()
{
  ios_base::sync_with_stdio(false);
  int t;
  cin >> t;
  for (int i = 0; i < t; ++i) {
    fill(ans, ans + n + m + 1, inf);
    solve();
    fill(dep, dep + n + m + 1, 0);
    fill(vis, vis + n + m + 1, false);
    fill(head, head + n + m + 1, nullptr);
    cur = ed;
  }
  return 0;
}