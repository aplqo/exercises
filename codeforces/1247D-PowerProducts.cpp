#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
using namespace std;
const unsigned int maxn = 1e5, maxk = 100, maxa = 1e5;
constexpr unsigned long long b = 131;

map<unsigned long long, unsigned int> cnt;
unsigned long long po[maxa + 10];
unsigned int mindiv[maxa + 10], primes[maxa + 10], *pcur = primes;
unsigned int pId[maxa + 10];
unsigned int a[maxn + 10];

inline void addHash(const unsigned long long h)
{
  const auto it = cnt.find(h);
  if (it != cnt.end())
    ++(it->second);
  else
    cnt[h] = 1;
}
inline unsigned int getCount(const unsigned long long h)
{
  const auto it = cnt.find(h);
  return it == cnt.end() ? 0 : it->second;
}
static void eular(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      pId[i] = pcur - primes;
      *(pcur++) = i;
      mindiv[i] = i;
    }
    for (const unsigned int* j = primes;
         j < pcur && 1ull * i * (*j) <= n && *j <= mindiv[i]; ++j)
      mindiv[*j * i] = *j;
  }
}
static void getPow(const unsigned int n)
{
  po[0] = 1;
  for (unsigned int i = 1; i <= n; ++i)
    po[i] = po[i - 1] * b;
}
pair<unsigned long long, unsigned long long> factor(unsigned int x,
                                                    const unsigned int k)
{
  unsigned long long hashX = 0,
                     hashC = 0;  // hash of x, hash of i ( i * x == t ^k)
  while (x > 1) {
    const unsigned int v = mindiv[x];
    unsigned int ex = 0;
    for (; mindiv[x] == v; x /= mindiv[x])
      ++ex;
    ex %= k;
    hashX += po[pId[v]] * ex;
    hashC += po[pId[v]] * ((k - ex) % k);
  }
  return make_pair(hashX, hashC);
}
unsigned long long count(const unsigned int n, const unsigned int k)
{
  unsigned long long ret = 0;
  for (const unsigned int* i = a; i < a + n; ++i) {
    const auto [hx, hc] = factor(*i, k);
    ret += getCount(hc);
    addHash(hx);
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, k;
  cin >> n >> k;
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  eular(*max_element(a, a + n));
  getPow(n);
  cout << count(n, k) << "\n";
  return 0;
}
