#include <climits>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
using num_t = uint_fast32_t;
const int maxn = 1000;
constexpr num_t mod = (num_t(1) << 31) - 1;

struct vertex;
struct edge {
  vertex* to;
  mutable bool vis = false;
  unsigned int val;
  edge *pre = nullptr, *rev;
} ed[maxn * maxn + 10], *cur = ed;
struct vertex {
  edge* head = nullptr;
  mutable bool vis = false;
  unsigned int f = 0, dis = UINT_MAX;
} ve[maxn + 10];
unsigned int n;
num_t ans = 0;

inline edge* AddEdge(const unsigned int from, const unsigned int to,
                     const unsigned int val)
{
  cur->to = ve + to;
  cur->val = val;
  cur->pre = ve[from].head;
  ve[from].head = cur;
  return cur++;
}
inline void dijkstra()
{
  using pair_t = pair<unsigned int, vertex*>;
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
unsigned int test(const vertex* x)
{
  unsigned int ret = 1;
  x->vis = true;
  for (edge* i = x->head; i; i = i->pre) {
    if ((!(i->vis || i->rev->vis)) || i->to->vis) continue;
    if (i->val + x->dis != i->to->dis) {
      x->vis = false;
      return 0;
    }
    unsigned int t = test(i->to);
    if (!t) {
      x->vis = false;
      return 0;
    }
    ret += t;
  }
  x->vis = false;
  return ret;
}
void dfs(const edge* p, const unsigned int k)
{
  if (!k) {
    if (test(ve + 1) == n) ++ans;
    return;
  }
  if (p >= cur) return;
  dfs(p + 1, k);
  if (p > p->rev) {
    p->vis = true;
    dfs(p + 1, k - 1);
    p->vis = false;
  }
}
inline void solve(ifstream& is)
{
  unsigned int m;
  is >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b, w;
    edge *ea, *eb;
    is >> a >> b >> w;
    ea = AddEdge(a, b, w);
    eb = AddEdge(b, a, w);
    ea->rev = eb;
    eb->rev = ea;
  }

  dijkstra();
  for (vertex* i = ve; i < ve + n + 1; ++i)
    i->vis = false;
  dfs(ed, n - 1);
  ans &= mod;
}
int main(int argc, char* argv[])
{
  ifstream in(argv[1]), out(argv[2]);
  solve(in);
  num_t res;
  out >> res;
  if (res == ans) {
    cout << "Test passed." << endl;
    return 0;
  }
  cout << "Test failed!" << endl
       << "\tRead=" << res << " Expect=" << ans << endl;
  return 0;
}
