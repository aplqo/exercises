/*Luogu team T132595: Number collect*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cctype>
#include <cstdio>
#include <map>
using namespace std;
using ans_t = unsigned long long;
const int maxv = 1e5, maxp = 10000, maxf = 20;

unsigned int mindiv[maxv + 10], primes[maxp + 10], *pcur = primes;
unsigned int k, cnt;
unsigned int tmp[maxf + 10], *ten = tmp;
map<unsigned int, unsigned int> cols;
map<unsigned long long, unsigned int> fac;
ans_t ans;

template <class T>
inline void read(T& dat)
{
  dat = 0;
  char c = getchar();
  while (!isdigit(c))
    c = getchar();
  while (isdigit(c)) {
    dat = dat * 10 + c - '0';
    c = getchar();
  }
}
// map operations
template <class T, class U>
inline void increase(map<T, U>& m, typename map<T, U>::key_type v)
{
  auto it = m.try_emplace(v, 0).first;
  ++(it->second);
}
template <class T, class U>
inline void decrease(map<T, U>& m, typename map<T, U>::key_type v)
{
  auto it = m.find(v);
  --(it->second);
  if (!it->second) m.erase(it);
}

void eular(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
    }
    for (unsigned int* j = primes; j < pcur && *j * i <= n; ++j)
      mindiv[*j * i] = *j;
  }
}
void factor(unsigned int n)
{
  ten = tmp;
  while (n > 1) {
    const unsigned int m = mindiv[n];
    *(ten++) = m;
    while (n % m == 0)
      n /= m;
  }
}
template <class T>
void dfs(const unsigned int* const p, T fun, const unsigned int dep = 0,
         const unsigned long long cur = 1)
{
  if (p >= ten) {
    if (dep) fun(dep, cur);
    return;
  }
  dfs(p + 1, fun, dep + 1, *p * cur);
  dfs(p + 1, fun, dep, cur);
}
unsigned int delt(const unsigned int v)
{
  if (v == 1) return cnt - 1;
  long long ret = 0;
  dfs(tmp, [&ret](unsigned int d, unsigned long long c) {
    ret = d & 0x01 ? ret + fac[c] : ret - fac[c];
  });
  return cnt - ret;
}
void add(unsigned int b)
{
  if (b % k) return;
  ++cnt;
  b /= k;
  factor(b);
  dfs(tmp, [](unsigned int, unsigned long long c) { increase(fac, c); });
  ans += delt(b);
  increase(cols, b);
}
void remove(unsigned int b)
{
  if (b % k) return;
  b /= k;
  if (cols.find(b) == cols.end()) return;
  factor(b);
  ans -= delt(b);
  dfs(tmp, [](unsigned int, unsigned long long c) { decrease(fac, c); });
  decrease(cols, b);
  --cnt;
}
int main()
{
  unsigned int n;
  read(n);
  read(k);
  eular(maxv);
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int a, b;
    read(a);
    read(b);
    if (a)
      add(b);
    else
      remove(b);
    printf("%Ld\n", ans);
  }
  return 0;
}
