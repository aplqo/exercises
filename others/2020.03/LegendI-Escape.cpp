// luogu group: T125564
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
using dis_t = unsigned int;
using cnt_t = unsigned long long;
constexpr dis_t inf = numeric_limits<dis_t>::max();
constexpr cnt_t mod = 1e9 + 7;
const int maxn = 10000, maxm = 400000;

struct vertex;
struct edge {
  vertex *to;
  dis_t val;
  edge *pre = nullptr;
} ed[maxm * 2 + 2];
struct vertex {
  enum { ORIG = 0, REV = 1 };
  dis_t val = inf;
  cnt_t ans = 0;
  bool vis = false;
  edge *head[2] = {};

  void AddEdge(const unsigned int to, dis_t val);

 private:
  static edge *ec;
  void add(const unsigned int typ, vertex *to, const dis_t val)
  {
    ec->to = to;
    ec->val = val;
    ec->pre = head[typ];
    head[typ] = ec;
    ++ec;
  }
} ve[maxn + 10];
edge *vertex::ec = ed;
void vertex::AddEdge(const unsigned int to, const dis_t val)
{
  add(ORIG, ve + to, val);
  ve[to].add(REV, this, val);
}

inline bool dijkstra(const unsigned int n)
{
  using pair_t = pair<dis_t, vertex *>;
  priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
  ve[1].val = 0;
  q.push(make_pair(0, ve + 1));
  while (!q.empty()) {
    const pair_t cur = q.top();
    q.pop();
    vertex &vc = *cur.second;
    if (vc.vis) continue;
    vc.vis = true;
    for (const edge *i = vc.head[vertex::ORIG]; i; i = i->pre)
      if (i->to->val > cur.first + i->val) {
        i->to->val = cur.first + i->val;
        q.push(make_pair(i->to->val, i->to));
      }
  }
  return ve[n].val < inf;
}
cnt_t dfs(vertex *const x)
{
  x->vis = false;
  for (edge *i = x->head[vertex::REV]; i; i = i->pre) {
    vertex *to = i->to;
    if (to->val + i->val == x->val)
      x->ans = (x->ans + (to->vis ? dfs(to) : to->ans)) % mod;
  }
  return x->ans;
}
int main()
{
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    dis_t c;
    cin >> a >> b >> c;
    ve[a].AddEdge(b, c);
  }
  if (!dijkstra(n)) {
    cout << "WTMGAY!!!" << endl;
    return 0;
  }
  ve[1].ans = 1;
  cout << ve[n].val << " " << dfs(ve + n) << endl;
  return 0;
}