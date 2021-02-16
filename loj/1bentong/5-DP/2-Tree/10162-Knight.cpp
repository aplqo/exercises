#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 1000000;

template <class T, size_t siz>
class Array {
 public:
  void push_back(T v) { *(cur++) = v; }
  T* end() { return cur; }
  T* begin() { return dat + 1; }
  T& operator[](const size_t s) { return dat[s]; }
  void clear() { cur = dat + 1; }

 private:
  T dat[siz + 1], *cur = dat + 1;
};
struct edge {
  unsigned int to;
  const edge *pre, *rev;
} ed[maxn * 2 + 1];
const edge* head[maxn + 1];
Array<unsigned int, maxn + 1> cyc;
enum { None = 0, Visit, Cycle, Last } vis[maxn + 1] = {};
enum class ReturnVal { Found, InCycle, NotFound };
unsigned long long val[maxn + 1], f[maxn + 1][2];

edge* addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  return cur++;
}
ReturnVal dfs(const unsigned int x, const edge* const f)
{
  vis[x] = Visit;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (i == f) continue;
    if (vis[to] == Visit) {
      vis[to] = Last;
      vis[x] = Cycle;
      cyc.push_back(x);
      return ReturnVal::InCycle;
    }
    switch (dfs(to, i->rev)) {
      case ReturnVal::Found:
        return ReturnVal::Found;
      case ReturnVal::InCycle:
        cyc.push_back(x);
        if (vis[x] == Last) {
          vis[x] = Cycle;
          return ReturnVal::Found;
        }
        vis[x] = Cycle;
        return ReturnVal::InCycle;
    }
  }
  return ReturnVal::NotFound;
}
void dfsSum(const unsigned int x)
{
  f[x][1] = val[x];
  vis[x] = Visit;
  for (const edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (f[to][1] || vis[to] == Cycle) continue;
    dfsSum(to);
    f[x][1] += f[to][0];
    f[x][0] += max(f[to][0], f[to][1]);
  }
}
unsigned long long dpOnce()
{
  unsigned long long stat[2] = {f[cyc[1]][0], 0};
  for (auto i = cyc.begin() + 1; i < cyc.end(); ++i) {
    const unsigned long long t1 = max(stat[0], stat[1]) + f[*i][0];
    const unsigned long long t2 = f[*i][1] + stat[0];
    stat[0] = t1;
    stat[1] = t2;
  }
  return max(stat[0], stat[1]);
}
unsigned long long dp()
{
  unsigned long long t1 = dpOnce();
  reverse(cyc.begin(), cyc.end());
  return max(t1, dpOnce());
}
unsigned long long solve(const unsigned int x)
{
  cyc.clear();
  dfs(x, 0);
  for (auto i : cyc) {
    dfsSum(i);
    vis[i] = Cycle;
  }
  return dp();
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
    cin >> val[i] >> t;
    edge* a = addEdge(i, t);
    edge* b = addEdge(t, i);
    a->rev = b;
    b->rev = a;
  }
  unsigned long long ret = 0;
  for (unsigned int i = 1; i <= n; ++i)
    if (vis[i] == None) ret += solve(i);
  cout << ret << endl;
  return 0;
}