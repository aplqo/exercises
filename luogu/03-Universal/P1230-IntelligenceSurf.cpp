#ifdef APDEBUG
#define COLOR
#define APINPROG
#include "debug_tools/time.cpp"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const int maxn = 505;

int pre[maxn], m, n;
struct game {
  int w, t;
} g[maxn];

int GetFree(int x) { return pre[x] == x ? x : pre[x] = GetFree(pre[x]); }
int main()
{
  ios_base::sync_with_stdio(false);
  cin >> m >> n;
  iota(pre, pre + n + 1, 0);
  for_each(g, g + n, [](game& t) -> void { cin >> t.t; });
  for_each(g, g + n, [](game& t) -> void { cin >> t.w; });
  sort(g, g + n,
       [](const game& a, const game& b) -> bool { return a.w > b.w; });
  for (game* i = g; i < g + n; ++i) {
    int t = GetFree(i->t);
    if (t)
      pre[t] = pre[t - 1];
    else
      m -= i->w;
  }
  cout << m;
  return 0;
}