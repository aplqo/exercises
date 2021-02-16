#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;
const unsigned int maxn = 50;
const unsigned int maxv = maxn * 2 * 2 + 2, maxe = maxv * maxv;

struct Edge {
  unsigned int from, to, id;
  int cap, flw;
  Edge* pre;
} ed[maxe + 10];
Edge *head[maxv + 10], *par[maxv + 10], *cur = ed;
unsigned int dep[maxv + 10];

void addDirEdge(const unsigned int from, const unsigned int to, const int cap)
{
  cur->from = from;
  cur->to = to;
  cur->id = cur - ed;
  cur->cap = cap;
  cur->flw = 0;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
inline void addEdge(const unsigned int from, const unsigned int to,
                    const int cap)
{
  addDirEdge(from, to, cap);
  addDirEdge(to, from, 0);
}
void bfs(const unsigned int t)
{
  static bool vis[maxv + 1];
  queue<unsigned int> q;
  memset(vis, 0, sizeof(vis));
  q.push(t);
  dep[t] = 0;
  vis[t] = true;
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (Edge* i = head[cur]; i; i = i->pre)
      if (!vis[i->to] && ed[i->id ^ 1].cap > ed[i->id ^ 1].flw) {
        vis[i->to] = true;
        dep[i->to] = dep[cur] + 1;
        q.push(i->to);
      }
  }
}
int augment(const unsigned int s, const unsigned int t)
{
  int ret = INT_MAX;
  unsigned int c = t;
  while (c != s) {
    ret = min(ret, par[c]->cap - par[c]->flw);
    c = par[c]->from;
  }
  c = t;
  while (c != s) {
    par[c]->flw += ret;
    ed[par[c]->id ^ 1].flw -= ret;
    c = par[c]->from;
  }
  return ret;
}
int isap(const unsigned int s, const unsigned int t, const unsigned int n)
{
  static unsigned int cnt[maxv + 1];
  static Edge* cur[maxv + 1];
  bfs(t);
  for (unsigned int i = 0; i <= n * 4 + 1; ++i)
    ++cnt[dep[i]];
  copy(head, head + n * 4 + 2, cur);
  int ret = 0;
  unsigned int x = s;
  while (dep[s] < n * 4 + 2) {
    if (x == t) {
      ret += augment(s, t);
      x = s;
    }
    bool adv = false;
    for (Edge* i = cur[x]; i; i = i->pre)
      if (dep[i->to] + 1 == dep[x] && i->cap > i->flw) {
        adv = true;
        par[i->to] = i;
        cur[x] = i;
        x = i->to;
        break;
      }
    if (!adv) {
      unsigned int mv = n * 4 + 2;
      for (Edge* i = head[x]; i; i = i->pre)
        if (i->cap > i->flw) mv = min(mv, dep[i->to] + 1);
      --cnt[dep[x]];
      if (!cnt[dep[x]]) break;
      dep[x] = mv;
      ++cnt[mv];
      cur[x] = head[x];
      if (x != s) x = par[x]->from;
    }
  }
  return ret;
}
void buildGraph(const unsigned int n, const unsigned int k)
{
  for (unsigned int i = 0; i < n; ++i) {
    static char str[maxn + 1];
    cin >> str;
    for (unsigned int j = 0; j < n; ++j)
      if (str[j] == 'Y')
        addEdge(i * 2, (j + n) * 2, 1);
      else
        addEdge(i * 2 + 1, (j + n) * 2 + 1, 1);
  }
  for (unsigned int i = 0; i < n; ++i)
    addEdge(i * 2, i * 2 + 1, k);
  for (unsigned int j = n; j < n * 2; ++j)
    addEdge(j * 2 + 1, j * 2, k);
  for (unsigned int i = 0; i < n; ++i)
    addEdge(n * 4, i * 2, 0);
  for (unsigned int j = n; j < n * 2; ++j)
    addEdge(j * 2, n * 4 + 1, 0);
}
void rebuild(const unsigned int n, const unsigned int v)
{
  for (Edge* i = head[n * 4]; i; i = i->pre)
    i->cap = v;
  for (Edge* i = head[n * 4 + 1]; i; i = i->pre)
    ed[i->id ^ 1].cap = v;
  for (Edge* i = ed; i < cur; ++i)
    i->flw = 0;
}
inline bool check(const unsigned int n, const int v)
{
  rebuild(n, v);
  return isap(n * 4, n * 4 + 1, n) == n * v;
}
unsigned int binary(const unsigned int n)
{
  int l = 0, r = n;
  while (l <= r) {
    const unsigned int mid = (l + r) >> 1;
    if (check(n, mid))
      l = mid + 1;
    else
      r = mid - 1;
  }
  return r;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  buildGraph(n, k);
  cout << binary(n) << endl;
  return 0;
}