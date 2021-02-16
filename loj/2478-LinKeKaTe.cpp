#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
using namespace std;
const unsigned int maxn = 3e5;
const long long maxe = 1e6;
constexpr long long ninf = LLONG_MIN / 4;

enum { Exclude = 0, Isolate = 1, Endpoint = 2, Lca = 3 };
struct Edge {
  unsigned int to;
  long long val;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
struct State {
  unsigned int cnt;
  long long val;
  inline bool operator<(const State& r) const
  {
    return val != r.val ? val < r.val : cnt < r.cnt;
  }
  inline State operator+(const State& r) const
  {
    return State{cnt + r.cnt, val + r.val};
  }
  inline State operator-(const State r) const
  {
    return State{cnt - r.cnt, val - r.val};
  }
  inline State operator+(const long long v) const
  {
    return State{cnt, val + v};
  }
};
State stat[maxn + 10][4];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const long long val)
{
  static Edge* cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, val, head[to]};
  head[to] = cur++;
}
void dfs(const unsigned int x, const unsigned int f, const State& scst)
{
  static constexpr State nil{0, 0};
  stat[x][Exclude] = nil;
  stat[x][Endpoint] = stat[x][Lca] = State{0, ninf};
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    dfs(to, x, scst);

    const State mc = max(*max_element(stat[to], stat[to] + 4), nil);
    const State mv = max(stat[to][Isolate], stat[to][Endpoint]) + i->val;
    stat[x][Lca] = max(stat[x][Endpoint] + mv - scst, stat[x][Lca] + mc);
    stat[x][Endpoint] = max(stat[x][0] + mv, stat[x][Endpoint] + mc);
    stat[x][Exclude] = stat[x][Exclude] + mc;
  }
  stat[x][Isolate] = stat[x][Exclude] + scst;
}
inline State maxChain(const long long v)
{
  dfs(1, 0, State{1, v});
  return *max_element(stat[1], stat[1] + 4);
}
long long binarySearch(const unsigned int n, const unsigned int k)
{
  long long l = -maxe * maxn, r = maxe * maxn;
  while (l <= r) {
    const long long mid = (l + r) / 2;
    const State st = maxChain(mid);
    if (st.cnt > k + 1)
      r = mid - 1;
    else if (st.cnt == k + 1)
      return st.val - mid * (k + 1);
    else
      l = mid + 1;
  }
  return maxChain(l).val - (k + 1) * l;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int x, y;
    long long z;
    cin >> x >> y >> z;
    addEdge(x, y, z);
  }
  cout << binarySearch(n, k) << "\n";
  return 0;
}
