#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxt = 25000, maxr = 50000, maxp = 50000;
constexpr long long inf = LLONG_MAX / 2;
using pair_t = pair<long long, unsigned int>;
using Heap = priority_queue<pair_t, vector<pair_t>, greater<pair_t>>;

struct Road {
  unsigned int to;
  long long val;
  const Road* pre;
} rd[maxr * 2];
const Road* headRoad[maxt + 1];
struct Plane {
  unsigned int from, to;
  long long val;
  const Plane* pre;
} pl[maxp + 1];
const Plane* headPlane[maxt + 1];
long long dis[maxt + 1];
Heap hp[maxt + 1];
bool vis[maxt + 1];
unsigned int blk[maxt + 1], degree[maxt + 1];

namespace dsu {
unsigned int fa[maxt + 1], siz[maxt + 1];
static void init(const unsigned int t)
{
  iota(fa, fa + 1 + t, 0);
  fill(siz, siz + 1 + t, 1);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
void merge(unsigned int x, unsigned int y)
{
  x = find(x);
  y = find(y);
  if (x == y) return;
  if (siz[x] < siz[y]) swap(x, y);
  fa[y] = x;
  siz[x] += siz[y];
}
}  // namespace dsu

inline void addRoad(const unsigned int from, const unsigned int to,
                    const long long val)
{
  static Road* rcur = rd;
  new (rcur) Road{to, val, headRoad[from]};
  headRoad[from] = rcur++;
  new (rcur) Road{from, val, headRoad[to]};
  headRoad[to] = rcur++;
  dsu::merge(from, to);
}
void dijkstra(const unsigned int b)
{
  Heap& q = hp[b];
  while (!q.empty()) {
    const pair_t cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const Road* i = headRoad[cur.second]; i; i = i->pre)
      if (dis[i->to] > cur.first + i->val) {
        dis[i->to] = cur.first + i->val;
        q.emplace(dis[i->to], i->to);
      }
  }
}
void topological(const unsigned int s, const unsigned int t)
{
  static bool reach[maxt + 1];
  queue<unsigned int> q;
  reach[blk[s]] = true;
  for (unsigned int i = 1; i <= t; ++i)
    if (blk[i] == i && !degree[blk[i]]) q.push(blk[i]);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    if (!reach[cur]) {
      for (const Plane* i = headPlane[cur]; i; i = i->pre) {
        const unsigned int tblk = blk[i->to];
        --degree[tblk];
        if (!degree[tblk]) q.push(tblk);
      }
      continue;
    }
    else {
      dijkstra(cur);
      for (const Plane* i = headPlane[cur]; i; i = i->pre) {
        const unsigned int to = i->to, tblk = blk[to];
        if (dis[to] > dis[i->from] + i->val) {
          dis[to] = dis[i->from] + i->val;
          hp[tblk].emplace(dis[to], to);
        }
        reach[tblk] = true;
        --degree[tblk];
        if (!degree[tblk]) q.push(tblk);
      }
    }
  }
}
static void buildGraph(const unsigned int t, const unsigned int p)
{
  for (unsigned int i = 1; i <= t; ++i)
    blk[i] = dsu::find(i);
  for (Plane* i = pl; i < pl + p; ++i) {
    const unsigned int blkFrom = blk[i->from], blkTo = blk[i->to];
    i->pre = headPlane[blkFrom];
    headPlane[blkFrom] = i;
    ++degree[blkTo];
  }
}
static void init(const unsigned int s, const unsigned int t)
{
  fill(dis, dis + 1 + t, inf);
  dis[s] = 0;
  hp[blk[s]].emplace(0, s);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t, p, r, s;
  cin >> t >> r >> p >> s;
  dsu::init(t);
  for (unsigned int i = 0; i < r; ++i) {
    unsigned int a, b, c;
    cin >> a >> b >> c;
    addRoad(a, b, c);
  }
  for (Plane* i = pl; i < pl + p; ++i)
    cin >> i->from >> i->to >> i->val;
  buildGraph(t, p);
  init(s, t);
  topological(s, t);
  for (unsigned int i = 1; i <= t; ++i)
    if (dis[i] != inf)
      cout << dis[i] << "\n";
    else
      cout << "NO PATH"
           << "\n";
  return 0;
}