#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
using namespace std;
const unsigned int maxn = 50, maxm = 50;
const unsigned int maxv = maxn + maxm + 1,
                   maxe = (maxn * maxm + maxn + maxm) * 2;
using cost_t = int;
constexpr cost_t inf = numeric_limits<cost_t>::max();

struct Edge {
  unsigned int from, to;
  cost_t cap;
  mutable cost_t flw = 0;
  const Edge *pre, *rev;
} ed[maxe + 1];
const Edge *head[maxv + 1], *par[maxv + 1];
unsigned int dep[maxv + 1];

void addDirEdge(const unsigned int from, const unsigned int to,
                const cost_t cap)
{
  static Edge* cur = ed;
  cur->from = from;
  cur->to = to;
  cur->cap = cap;
  cur->rev = &ed[(cur - ed) ^ 1];
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void addEdge(const unsigned int from, const unsigned int to,
                    const cost_t cap)
{
  addDirEdge(from, to, cap);
  addDirEdge(to, from, 0);
}
void bfs(const unsigned int t, const unsigned int n)
{
  static bool vis[maxv + 1];
  memset(vis, 0, sizeof(vis));
  fill(dep, dep + n, n);
  queue<unsigned int> q;
  q.push(t);
  dep[t] = 0;
  vis[t] = true;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (const Edge* i = head[cur]; i; i = i->pre)
      if (!vis[i->to] && i->rev->cap > i->rev->flw) {
        vis[i->to] = true;
        dep[i->to] = dep[cur] + 1;
        q.push(i->to);
      }
  }
}
cost_t augment(const unsigned int s, const unsigned int t)
{
  cost_t ret = inf;
  unsigned int x = t;
  while (x != s) {
    ret = min(ret, par[x]->cap - par[x]->flw);
    x = par[x]->from;
  }
  x = t;
  while (x != s) {
    par[x]->flw += ret;
    par[x]->rev->flw -= ret;
    x = par[x]->from;
  }
  return ret;
}
cost_t isap(const unsigned int s, const unsigned int t, const unsigned int n)
{
  static unsigned int cnt[maxv + 1];
  static const Edge* cur[maxv + 1];
  bfs(t, n);
  for (unsigned int i = 0; i < n; ++i)
    ++cnt[dep[i]];
  copy(head, head + n, cur);
  unsigned int x = s;
  cost_t ret = 0;
  while (dep[s] < n) {
    if (x == t) {
      ret += augment(s, t);
      x = s;
    }
    bool adv = false;
    for (const Edge* i = cur[x]; i; i = i->pre)
      if (dep[i->to] + 1 == dep[x] && i->cap > i->flw) {
        adv = true;
        par[i->to] = i;
        cur[x] = i;
        x = i->to;
        break;
      }
    if (!adv) {
      unsigned int mv = n;
      for (const Edge* i = head[x]; i; i = i->pre)
        if (i->cap > i->flw) mv = min(mv, dep[i->to] + 1);
      --cnt[dep[x]];
      if (!cnt[dep[x]]) break;
      cur[x] = head[x];
      dep[x] = mv;
      ++cnt[dep[x]];
      if (x != s) x = par[x]->from;
    }
  }
  return ret;
}
cost_t buildraph(const unsigned int n, const unsigned int m)
{
  const unsigned int s = n + m, t = n + m + 1;
  cost_t sum = 0;
  cin.ignore(2, '\n');
  for (unsigned int i = 0; i < m; ++i) {
    string buf;
    getline(cin, buf);
    istringstream is(buf);
    cost_t c;
    is >> c;
    sum += c;
    addEdge(s, i, c);
    while (is) {
      unsigned int t;
      is >> t;
      if (is.fail()) break;
      addEdge(i, t - 1 + m, inf);
    }
  }
  for (unsigned int i = 0; i < n; ++i) {
    cost_t c;
    cin >> c;
    addEdge(m + i, t, c);
  }
  return sum;
}
void printAns(const unsigned int n, const unsigned int m)
{
  const unsigned int v = n + m + 2;
  bfs(n + m + 1, v);
  bool fl = false;
  for (unsigned int i = 0; i < m; ++i)
    if (dep[i] == v) {
      if (fl) cout << " ";
      cout << (i + 1);
      fl = true;
    }
  cout << endl;
  fl = false;
  for (unsigned int j = 0; j < n; ++j)
    if (dep[m + j] == v) {
      if (fl) cout << " ";
      cout << (j + 1);
      fl = true;
    }
  cout << endl;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int m, n;
  cin >> m >> n;
  const cost_t s = buildraph(n, m);
  const cost_t ans = s - isap(n + m, n + m + 1, n + m + 2);
  printAns(n, m);
  cout << ans << endl;
  return 0;
}