#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <queue>
#include <random>
#include <utility>
const int maxnm = 233, maxk = 5;
const int base = 2000, iterateTime = 150;
const int dx[] = {0, 0, 1, -1}, dy[] = {1, -1, 0, 0};
const int inf = INT_MAX / 3;

struct {
  int next[4], *end;
} next[maxnm];
int color[maxnm + 10], a[maxnm + 10], weight[maxnm + 10];
unsigned int mapMem[maxnm + 10], *map = mapMem;

namespace minSteinerTree {
struct Node {
  int dis, pos;
  inline bool operator<(const Node &r) const { return dis > r.dis; }
};
const unsigned int maxMsk = 1u << maxk;
int dis[maxMsk][maxnm + 10], vis[maxMsk][maxnm + 10];

void dijkstra(const int size, const unsigned int msk)
{
  static int sta = 0;
  std::priority_queue<Node> q;
  int *const cdis = dis[msk], *const cvis = vis[msk];
  ++sta;
  for (int i = 0; i < size; ++i)
    if (cdis[i] != inf) q.push(Node{cdis[i], i});
  while (!q.empty()) {
    const Node cur = q.top();
    q.pop();
    if (cvis[cur.pos] == sta) continue;
    cvis[cur.pos] = sta;
    const int nxt = cur.dis + weight[cur.pos];
    for (const int *i = next[cur.pos].next; i < next[cur.pos].end; ++i)
      if (cdis[*i] > nxt) {
        cdis[*i] = nxt;
        q.push(Node{nxt, *i});
      }
  }
}
int minSteinerTree(const int size, const unsigned int msk)
{
  for (unsigned int i = 0; i < msk; ++i)
    std::fill(dis[i], dis[i] + size, inf);
  for (int i = 0; i < size; ++i)
    if (color[i] != -1) dis[map[color[i]]][i] = dis[0][i] = 0;
  for (unsigned int i = 1; i < msk; ++i) {
    for (unsigned int k = i; k; k = (k - 1) & i)
      for (int j = 0; j < size; ++j)
        dis[i][j] = std::min(dis[i][j], dis[k][j] + dis[k ^ i][j]);
    dijkstra(size, i);
  }
  int ret = inf;
  for (int i = 0; i < size; ++i)
    ret = std::min(ret, dis[msk - 1][i] + weight[i]);
  return ret;
}
}  // namespace minSteinerTree

std::mt19937 rnd((std::random_device())());
void initMap(const int c, const int k)
{
  std::uniform_int_distribution<int> dis(0, k - 1);
  map = mapMem;
  for (int i = 0; i < k; ++i)
    map[i] = 1u << i;
  for (unsigned int *i = map + k; i < map + c; ++i)
    *i = 1u << dis(rnd);
  std::shuffle(map, map + c, rnd);
}
void remap(const int c) { std::shuffle(map, map + c, rnd); }
int minConnectBlock(const int size, const int c, const int k)
{
  const unsigned int msk = 1u << k;
  int ret = inf;
  for (int i = iterateTime; i; --i) {
    initMap(c, k);
    ret = std::min(ret, minSteinerTree::minSteinerTree(size, msk));
  }
  return ret;
}
std::pair<int, int> solve(const int size, const int mxc, const int k)
{
  for (int i = 0; i < size; ++i)
    weight[i] = color[i] == -1 ? inf : 1;
  const int cnt = minConnectBlock(size, mxc, k);
  if (cnt >= inf) return std::make_pair(-1, -1);

  int l = 0, r = *std::max_element(a, a + size);
  while (l <= r) {
    const int mid = (l + r) / 2;
    for (int i = 0; i < size; ++i)
      if (color[i] != -1)
        weight[i] = a[i] < mid ? base - 1 : base + 1;
      else
        weight[i] = inf;
    const int c = minConnectBlock(size, mxc, k);
    if (c <= (c + size) / base * base)
      r = mid - 1;
    else
      l = mid + 1;
  }
  return std::make_pair(cnt, r);
}

static void buildGraph(const int n, const int m)
{
  for (int i = 0, acc = 0; i < n; ++i)
    for (int j = 0; j < m; ++j, ++acc) {
      next[acc].end = next[acc].next;
      if (color[acc] == -1) continue;
      for (int k = 0; k < 4; ++k) {
        const int nx = i + dx[k], ny = j + dy[k];
        if (0 <= nx && nx < n && 0 <= ny && ny < m && color[nx * m + ny] != -1)
          *(next[acc].end++) = nx * m + ny;
      }
    }
}
int read(const int n, const int m)
{
  int ret = 0;
  for (int i = 0, acc = 0; i < n; ++i)
    for (int j = 0; j < m; ++j, ++acc) {
      std::cin >> color[acc];
      if (color[acc] != -1) {
        ret = std::max(ret, color[acc]);
        --color[acc];
      }
    }
  for (int i = 0, acc = 0; i < n; ++i)
    for (int j = 0; j < m; ++j, ++acc)
      std::cin >> a[acc];
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = t; i; --i) {
    int n, m, k;
    std::cin >> n >> m >> k;
    const int mc = read(n, m);
    buildGraph(n, m);
    const auto [cnt, mid] = solve(n * m, mc, k);
    std::cout << cnt << " " << mid << "\n";
  }
  return 0;
}