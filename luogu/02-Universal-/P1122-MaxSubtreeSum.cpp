#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 16000;
constexpr int ninf = INT_MIN / 2;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxn * 2 + 1];
edge* head[maxn + 1];
int nice[maxn + 1], ans = ninf;

void addEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
int dfs(const unsigned int x, const unsigned int f)
{
  int ret = nice[x];
  for (edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const int v = dfs(to, x);
    ret = max(ret, ret + v);
    ans = max({ans, v, ret});
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  copy_n(istream_iterator<int>(cin), n, nice + 1);
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    addEdge(a, b);
    addEdge(b, a);
  }
  dfs(1, 0);
  cout << ans << endl;
  return 0;
}