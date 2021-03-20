#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
const int maxV = 1e6, maxN = 1e5, maxQ = 1e5;
const unsigned int mod = 1e9 + 7;

struct Number {
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{static_cast<unsigned int>(
        (static_cast<unsigned long long>(l.val) * r.val) % mod)};
  }

  unsigned int val;
};

bool exist[maxV + 10];
std::vector<unsigned int> factor[maxV + 10];
Number dCk[maxN + maxQ + 10], inv[maxN + maxQ + 10];
struct {
  int cnt;
  Number phi;
} fac[maxV + 10];

static void eular(const int n)
{
  static int mindiv[maxV + 10], primes[maxV + 10], *pcur = primes;
  fac[1].phi.val = 1;
  for (long long i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
      fac[i].phi.val = i - 1;
    }
    for (const int *j = primes; j < pcur && *j < mindiv[i] && *j * i <= n;
         ++j) {
      const int v = *j * i;
      mindiv[v] = *j;
      fac[v].phi.val = fac[i].phi.val * (*j - 1);
    }
    if (const long long v = mindiv[i] * i; v <= n) {
      mindiv[v] = mindiv[i];
      fac[v].phi.val = fac[i].phi.val * mindiv[i];
    }
  }
}
static void initFactor(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    for (unsigned int j = i; j <= n; j += i)
      if (exist[j]) factor[j].push_back(i);
}
static void initModularInv(const unsigned int n)
{
  inv[1].val = 1;
  for (unsigned int i = 2; i <= n; ++i)
    inv[i].val = mod - (inv[mod % i] * Number{mod / i}).val;
}
static void initCK(const unsigned int n, const unsigned int k)
{
  initModularInv(n);
  dCk[k].val = 1;
  for (unsigned int i = k + 1; i <= n; ++i)
    dCk[i] = dCk[i - 1] * Number{i} * inv[i - k];
  std::adjacent_difference(dCk + k, dCk + n + 1, dCk + k);
}
static void init(const int v, const int k, const int n)
{
  eular(v);
  initFactor(v);
  initCK(n, k);
}

Number addNumber(const int p)
{
  Number ret{0};
  for (unsigned int j : factor[p])
    ret += fac[j].phi * dCk[++fac[j].cnt];
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  static int buf[maxN + maxQ + 10];
  int n, k, q, max = 0;
  std::cin >> n >> k >> q;
  for (int *i = buf; i < buf + n + q; ++i) {
    std::cin >> *i;
    max = std::max(*i, max);
    exist[*i] = true;
  }
  Number ans{0};
  init(max, k, n + q);
  for (const int *i = buf; i < buf + n; ++i)
    ans += addNumber(*i);
  for (const int *i = buf + n; i < buf + n + q; ++i) {
    ans += addNumber(*i);
    std::cout << ans.val << "\n";
  }
  return 0;
}