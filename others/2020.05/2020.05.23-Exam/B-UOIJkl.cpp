/*Luogu team T134375: UOI Jin ke la*/
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
using num = unsigned int;
const unsigned int maxn = 30, maxm = 16;
constexpr num inf = numeric_limits<num>::max();

num f[(1 << maxm) + 1];
bool vis[(1 << maxm) + 1];
struct {
  unsigned int del = 0;
  unsigned int add = 0;
} ch[maxn + 10];

void dijkstra(const unsigned int ini, const unsigned int n,
              const unsigned int m)
{
  using pair_t = pair<num, unsigned int>;
  priority_queue<pair_t, vector<pair_t>, greater<pair_t>> q;
  fill(f, f + (1 << m), inf);
  f[ini] = 0;
  q.emplace(0, ini);
  while (!q.empty()) {
    const auto cur = q.top();
    q.pop();
    if (vis[cur.second]) continue;
    vis[cur.second] = true;
    for (const auto* i = ch; i < ch + n; ++i) {
      unsigned int t = (cur.second & i->del) | i->add;
      if (f[t] > cur.first + 1) {
        f[t] = cur.first + 1;
        q.emplace(f[t], t);
      }
    }
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  unsigned int ini = 0;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int t;
    cin >> t;
    if (t) ini |= 1 << i;
  }
  for (auto* i = ch; i < ch + n; ++i) {
    for (unsigned int j = 0; j < m; ++j) {
      int t;
      cin >> t;
      if (t == 1)
        i->add |= 1 << j;
      else if (t == -1)
        i->del |= 1 << j;
    }
    i->del = ~(i->del);
  }
  dijkstra(ini, n, m);
  if (f[0] == inf)
    cout << "-1" << endl;
  else
    cout << f[0] << endl;
  return 0;
}