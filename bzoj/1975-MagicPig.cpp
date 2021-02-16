#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
using num_t = double;
template <class T>
using heap_t = priority_queue<T, vector<T>, greater<T>>;
const int maxn = 5000, maxm = 200000;
constexpr num_t inf = numeric_limits<num_t>::max();

class graph {
 public:
  struct edge {
    unsigned int to;
    num_t val;
    edge* pre = nullptr;
  };
  inline const edge* GetEdge(const unsigned int x) const { return head[x]; }
  inline void AddEdge(const unsigned int from, const unsigned int to,
                      const num_t val)
  {
    cur->to = to;
    cur->val = val;
    cur->pre = head[from];
    head[from] = cur;
    ++cur;
  }

 private:
  edge ed[maxm + 10], *cur = ed;
  edge* head[maxn + 10] = {};
} gra, rev;
bool vis[maxn + 10];
num_t dis[maxn + 10];

inline void dijkstra(const unsigned int n)
{
  using pair_t = pair<num_t, unsigned int>;
  heap_t<pair_t> q;
  fill(dis, dis + n + 1, inf);
  dis[n] = 0;
  q.push(make_pair(0, n));
  while (!q.empty()) {
    pair_t cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (auto i = rev.GetEdge(cur.second); i; i = i->pre)
      if (dis[i->to] > cur.first + i->val) {
        dis[i->to] = cur.first + i->val;
        q.push(make_pair(dis[i->to], i->to));
      }
  }
}
inline unsigned int AStar(const unsigned int n, num_t e)
{
  using tuple_t = tuple<num_t, num_t, unsigned int>;
  heap_t<tuple_t> q;
  q.push(make_tuple(dis[1], 0, 1));
  unsigned int ret = 0;
  while (!q.empty()) {
    tuple_t cur = q.top();
    q.pop();
    if (get<2>(cur) == n) {
      if (get<0>(cur) <= e) {
        ++ret;
        e -= get<0>(cur);
      }
      else
        break;
    }
    for (auto i = gra.GetEdge(get<2>(cur)); i; i = i->pre) {
      const num_t f = get<1>(cur) + i->val;
      q.push(make_tuple(f + dis[i->to], f, i->to));
    }
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  num_t e;
  cin >> n >> m >> e;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int s, t;
    num_t e;
    cin >> s >> t >> e;
    gra.AddEdge(s, t, e);
    rev.AddEdge(t, s, e);
  }
  dijkstra(n);
  cout << AStar(n, e) << endl;
  return 0;
}