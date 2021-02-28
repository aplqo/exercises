#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <vector>
const int maxn = 1e6, maxa = 1e7;

int val[maxn + 10];
int mindiv[maxa + 10], primes[maxa + 10], *pcur = primes;
std::vector<int> pos[maxa + 10], factor[maxn + 10];
struct Range {
  int l, r;

  bool contains(const int l, const int r) const
  {
    return this->l <= l && r <= this->r;
  }
} maxCoPrime[maxn + 10];
int fa[maxn + 10];

static void eular(const int ma)
{
  for (long long i = 2; i <= ma; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
    }
    for (const int *j = primes; j < pcur && *j * i <= ma && *j <= mindiv[i];
         ++j)
      mindiv[*j * i] = *j;
  }
}
void factorVals(const int n)
{
  for (const int *i = primes; i < pcur; ++i)
    pos[*i].push_back(0);
  for (int i = 1; i <= n; ++i) {
    int v = val[i];
    while (v > 1) {
      pos[mindiv[v]].push_back(i);
      factor[i].push_back(mindiv[v]);
      const int c = mindiv[v];
      while (mindiv[v] == c)
        v /= mindiv[v];
    }
  }
  for (const int *i = primes; i < pcur; ++i)
    pos[*i].push_back(n + 1);
}
static void initRange(const int n)
{
  static struct PtrRange {
    const int *l, *r;
  } ptr[maxa + 10];
  factorVals(n);
  for (const int *i = primes; i < pcur; ++i)
    ptr[*i] = PtrRange{.l = &pos[*i][0], .r = &pos[*i][1]};
  for (int i = 1; i <= n; ++i) {
    int l = 1, r = n + 1;
    for (auto j : factor[i]) {
      l = std::max(l, *(ptr[j].l++) + 1);
      r = std::min(r, *(++ptr[j].r));
    }
    if (l > r) l = r;
    maxCoPrime[i] = Range{.l = l, .r = r};
  }
}

[[noreturn]] void fail()
{
  std::cout << "impossible\n";
  std::exit(0);
}
int solve(const int l, const int r)
{
  static const auto tryConstruct = [](const int rt, const int l, const int r) {
    if (maxCoPrime[rt].contains(l, r)) {
      if (l < rt) fa[solve(l, rt)] = rt;
      if (rt + 1 < r) fa[solve(rt + 1, r)] = rt;
      return true;
    }
    return false;
  };
  if (r - l == 1) return l;
  int pl = l, pr = r - 1;
  while (pl <= pr) {
    if (tryConstruct(pl, l, r)) return pl;
    if (tryConstruct(pr, l, r)) return pr;
    ++pl;
    --pr;
  }
  fail();
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, mxa = 0;
  std::cin >> n;
  for (int *i = val + 1; i < val + 1 + n; ++i) {
    std::cin >> *i;
    mxa = std::max(mxa, *i);
  }
  eular(mxa);
  initRange(n);
  solve(1, n + 1);
  std::copy(fa + 1, fa + 1 + n, std::ostream_iterator<int>(std::cout, " "));
  return 0;
}