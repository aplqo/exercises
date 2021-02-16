#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdint>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
using num_t = uint_fast32_t;
using val_t = unsigned int;
const int maxn = 1000;
constexpr num_t mod = 2147483647;

struct vertex;
struct edge {
  vertex* to;
  val_t val;
  edge* pre = nullptr;
} ed[(maxn * maxn) + 10];
struct vertex {
  edge* head = nullptr;
  mutable bool vis = false;
  unsigned int f = 0;
  val_t dis = UINT_MAX;
} ve[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to,
                    const val_t val)
{
  static edge* cur = ed;
  cur->to = ve + to;
  cur->val = val;
  cur->pre = ve[from].head;
  ve[from].head = cur;
  ++cur;
}
inline void dijkstra()
{
  using pair_t = pair<val_t, vertex*>;
  priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
  ve[1].dis = 0;
  q.push(make_pair(0, ve + 1));
  while (!q.empty()) {
    pair_t cur = q.top();
    q.pop();
    if (cur.second->vis) continue;
    cur.second->vis = true;
    for (edge* i = cur.second->head; i; i = i->pre)
      if (cur.first + i->val < i->to->dis) {
        i->to->dis = cur.first + i->val;
        q.push(make_pair(i->to->dis, i->to));
      }
  }
}
void dfs(vertex* const x)
{
  x->vis = true;
  for (const edge* i = x->head; i; i = i->pre) {
    if (i->to->dis + i->val == x->dis) ++(x->f);
    if (!i->to->vis) dfs(i->to);
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  num_t ans = 1;
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    val_t w;
    cin >> a >> b >> w;
    AddEdge(a, b, w);
    AddEdge(b, a, w);
  }

  dijkstra();
  for_each(ve, ve + n + 1, [](const vertex& i) { i.vis = false; });
  dfs(ve + 1);
  ve[1].f = 1;
  for (const vertex* i = ve + 1; i < ve + n + 1; ++i)
    ans = (ans * i->f) % mod;
  cout << ans << endl;
  return 0;
}
