#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
#include <queue>
using namespace std;
const unsigned int maxn = 40;
const unsigned int maxv = maxn * maxn * maxn + 2,
                   maxe = (maxn * maxn * maxn * 5 * 2 + maxn * maxn) * 2;
constexpr int inf = INT_MAX / 2;

struct Edge {
  unsigned int from, to;
  int cap;
  mutable int flw = 0;
  const Edge *pre, *rev;
} ed[maxe + 1];
const Edge *head[maxv + 1], *par[maxv + 1];
unsigned int dep[maxv + 1];
int mat[maxn + 1][maxn + 1][maxn + 1];

void addDirEdge(const unsigned int from, const unsigned int to, const int cap)
{
  static Edge* cur = ed;
  cur->from = from;
  cur->to = to;
  cur->cap = cap;
  cur->pre = head[from];
  head[from] = cur;
  cur->rev = &ed[(cur - ed) ^ 1];
  ++cur;
}
void addEdge(const unsigned int from, const unsigned int to, const int cap)
{
  addDirEdge(from, to, cap);
  addDirEdge(to, from, 0);
}
void bfs(const unsigned int t)
{
  static bool vis[maxv + 1];
  queue<unsigned int> q;
  q.push(t);
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
int agument(const unsigned int s, const unsigned int t)
{
  int ret = inf;
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
int isap(const unsigned int s, const unsigned int t, const unsigned int n)
{
  static const Edge* cur[maxv + 1];
  static unsigned int cnt[maxv + 1];
  bfs(t);
  for (unsigned int i = 0; i < n; ++i)
    ++cnt[dep[i]];
  copy(head, head + n + 1, cur);
  int ret = 0;
  unsigned int x = s;
  while (dep[x] < n) {
    if (x == t) {
      ret += agument(s, t);
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
      dep[x] = mv;
      ++cnt[dep[x]];
      cur[x] = head[x];
      if (x != s) x = par[x]->from;
    }
  }
  return ret;
}
pair<unsigned int, unsigned int> buildGraph(const int p, const int q,
                                            const int r, const int d)
{
  static const int dx[] = {0, 0, -1, 1}, dy[] = {1, -1, 0, 0};
  const auto id = [p, q](const int x, const int y, const int z) -> int {
    return z * p * q + y * p + x;
  };
  const int s = id(p, q, r) + 1, t = s + 1;
  for (int i = 0; i < p; ++i)
    for (int j = 0; j < q; ++j) {
      addEdge(s, id(i, j, 0), inf);
      for (int k = 1; k <= r; ++k) {
        const int cp = id(i, j, k);
        addEdge(id(i, j, k - 1), cp, mat[i][j][k - 1]);
        if (k > d)
          for (unsigned int v = 0; v < 4; ++v) {
            const int x = i + dx[v], y = j + dy[v];
            if (x >= 0 && y >= 0 && x < p && y < q)
              addEdge(cp, id(x, y, k - d), inf);
          }
      }
      addEdge(id(i, j, r), t, inf);
    }
  return make_pair(s, t);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int p, q, r, d;
  cin >> p >> q >> r >> d;
  for (unsigned int i = 0; i < r; ++i)
    for (unsigned int j = 0; j < p; ++j)
      for (unsigned int k = 0; k < q; ++k)
        cin >> mat[j][k][i];
  const auto [s, t] = buildGraph(p, q, r, d);
  cout << isap(s, t, t) << endl;
  return 0;
}