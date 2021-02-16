#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
using namespace std;
const int maxn = 1005, maxp = 10005;
typedef pair<int, int> pairType;
typedef priority_queue<pairType, vector<pairType>, greater<pairType>> que;

struct edge {
  int to, val;
  edge* pre = nullptr;
} ed[maxp * 2];
edge* head[maxn];
int ans[maxn];
bool vis[maxn];
int n, p, k;
int v;

inline void AddEdge(int x, int y, int val)
{
  static edge* cur = ed;
  cur->to = y;
  cur->val = val;
  cur->pre = head[x];
  head[x] = cur;
  ++cur;
}
bool dijkstra(int v)
{
  que q;
  q.push(make_pair(0, 1));
  fill(ans, ans + 1 + n, INT_MAX);
  fill(vis, vis + 1 + n, false);
  ans[1] = 0;
  while (!q.empty()) {
    pairType cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (edge* i = head[cur.second]; i; i = i->pre) {
      int tv = i->val > v ? 1 : 0;
      if (ans[i->to] > cur.first + tv) {
        ans[i->to] = cur.first + tv;
        q.push(make_pair(ans[i->to], i->to));
      }
    }
  }
  if (ans[n] == INT_MAX) {
    cout << "-1" << endl;
    exit(0);
  }
  return ans[n] <= k;
}
int main()
{
  ios_base::sync_with_stdio(false);
  int l = 0, r = 0, ret = 0;
  cin >> n >> p >> k;
  for (int i = 0; i < p; ++i) {
    int a, b, l;
    cin >> a >> b >> l;
    r = max(r, l);
    AddEdge(a, b, l);
    AddEdge(b, a, l);
  }
  while (l <= r) {
    int mid = (l + r) >> 1;
    if (dijkstra(mid))
      r = mid - 1;
    else
      l = mid + 1;
  }
  for (edge* i = ed; i < ed + p * 2; ++i)
    if (i->val <= l) ret = max(i->val, ret);
  cout << ret << endl;
  return 0;
}