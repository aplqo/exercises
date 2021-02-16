#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <map>
#include <numeric>
#include <tuple>
using namespace std;
using real = double;
const int maxn = 200;

struct {
  int a;
  real p;
} cha[maxn + 1] = {{0, 1}};
unsigned int n, l, pos;
int sum[maxn + 1];

real dfs(const unsigned int p, int v, const unsigned int cnt = 0)
{
  static map<tuple<unsigned int, int, unsigned int>, real> mem;
  if (p > n) return v >= 0 && !cnt ? 1 : 0;
  if (p >= pos && v >= 0 && !cnt) return 1;
  if (p >= pos && v + sum[n] - sum[p - 1] < 0 || cnt > n - p + 1) return 0;
  const auto&& cv = make_tuple(p, v, cnt);  // use rvalue reference
  {
    const auto it = mem.find(cv);
    if (it != mem.end()) return it->second;
  }
  return mem[cv] = dfs(p + 1, v + cha[p].a, cnt >= 1 ? cnt - 1 : 0) * cha[p].p +
                   dfs(p + 1, v, cnt) * (1 - cha[p].p);
}

int main()
{
  unsigned int k;
  scanf("%d%d%d", &n, &l, &k);
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int t;
    scanf("%d", &t);
    cha[i].p = t / 100.0;
  }
  for (unsigned int i = 1; i <= n; ++i)
    scanf("%d", &cha[i].a);
  sort(cha + 1, cha + n + 1, [](const auto& a, const auto& b) {
    return a.a != b.a ? a.a < b.a : a.p > b.p;
  });
  pos = find_if_not(cha + 1, cha + 1 + n,
                    [](const auto& i) { return i.a == -1; }) -
        cha;
  for (unsigned int i = pos; i <= n; ++i)
    sum[i] = sum[i - 1] + cha[i].a;
  printf("%.6lf\n", dfs(1, k, l));
  return 0;
}