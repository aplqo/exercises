#ifdef APTEST
#include "debug_tools/program.h"
#endif
#if !defined APTEST && !defined APDEBUG
#define NDEBUG
#endif
#include <algorithm>
#include <cassert>
#include <deque>
#include <iostream>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 1e6;

class Queue {
 public:
  void push(const unsigned int p, const long long val)
  {
    while (!q.empty() && q.back().second < val)
      q.pop_back();
    q.emplace_back(p, val);
  }
  void pop(const unsigned int p)
  {
    while (!q.empty() && q.front().first < p)
      q.pop_front();
  }
  long long front() const
  {
    assert(!q.empty());
    return q.front().second;
  }

 private:
  deque<pair<unsigned int, long long>> q;
};
template <class T, unsigned int siz>
class Array {
 public:
  void push_back(const T& v) { *(cur++) = v; };
  T* begin() { return val + 1; };
  T* end() { return cur; };
  void clear() { cur = val + 1; };
  T& operator[](const size_t i) { return val[i]; };
  void expand()
  {
    copy(val + 1, cur, cur);
    cur += cur - val - 1;
  }
  size_t size() const { return cur - val - 1; };

 private:
  T val[siz] = {}, *cur = val + 1;
};
struct edge {
  unsigned int to;
  long long val;
  edge *pre, *rev;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
enum { None = 0, Cycle, Visit, Last } vis[maxn + 1] = {};
enum dfsCycle { InCycle, NotFound, Found };
Array<unsigned int, maxn * 2 + 1> cyc;
Array<long long, maxn * 2 + 1> cycEdge;
long long len[maxn + 1];

edge* addEdge(const unsigned int from, const unsigned int to,
              const long long val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  return cur++;
}
dfsCycle dfs(const unsigned int x, const edge* const f)
{
  vis[x] = Visit;
  for (edge* i = head[x]; i; i = i->pre) {
    if (i == f) continue;
    if (vis[i->to] == Visit) {
      vis[i->to] = Last;
      vis[x] = Cycle;
      cyc.push_back(x);
      cycEdge.push_back(i->val);
      return InCycle;
    }
    switch (dfs(i->to, i->rev)) {
      case Found:
        return Found;
      case InCycle:
        cycEdge.push_back(i->val);
        cyc.push_back(x);
        if (vis[x] == Last) {
          vis[x] = Cycle;
          return Found;
        }
        vis[x] = Cycle;
        return InCycle;
    }
  }
  return NotFound;
}
void dfsPath(const unsigned int x, const unsigned int f, long long& ans)
{
  vis[x] = Visit;
  unsigned int p = 0;
  long long cans = 0;
  const auto upd = [&cans, &p, &x](const unsigned int to, const long long v) {
    cans = max(cans, len[x] + v);
    if (len[x] < v) {
      len[x] = v;
      p = to;
    }
  };
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f || vis[to] == Cycle) continue;
    if (!len[to]) {
      dfsPath(to, x, ans);
      upd(to, len[to] + i->val);
    }
    else {
      const long long v = len[to] + i->val;
      if (p != to) upd(to, v);
      if (len[x] < v && p == to) {
        len[x] = v;
        cans = cans + v - len[x];
      }
    }
  }
  ans = max(ans, cans);
}
long long dpCycle(const unsigned int n)
{
  Queue q;
  q.push(1, len[cyc[1]] - cycEdge[1]);
  long long ret = 0;
  for (unsigned int i = 2; i < n; ++i) {
    ret = max(ret, q.front() + cycEdge[i] + len[cyc[i]]);
    q.push(i, len[cyc[i]] - cycEdge[i]);
  }
  for (unsigned int i = n; i <= 2 * n; ++i) {
    q.pop(i + 1 - n);
    ret = max(ret, q.front() + cycEdge[i] + len[cyc[i]]);
    q.push(i, len[cyc[i]] - cycEdge[i]);
  }
  return ret;
}
long long solve(const unsigned int x)
{
  cyc.clear();
  cycEdge.clear();
  dfs(x, 0);
  long long ret = 0;
  for (auto i : cyc) {
    dfsPath(i, 0, ret);
    vis[i] = Cycle;
  }
  cyc.expand();
  cycEdge.expand();
  partial_sum(cycEdge.begin(), cycEdge.end(), cycEdge.begin());
  return max(ret, dpCycle(cyc.size() / 2));
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int t;
    long long l;
    cin >> t >> l;
    edge *a = addEdge(i, t, l), *b = addEdge(t, i, l);
    a->rev = b;
    b->rev = a;
  }
  long long ans = 0;
  for (unsigned int i = 1; i <= n; ++i)
    if (vis[i] == None) ans += solve(i);
  cout << ans << endl;
  return 0;
}