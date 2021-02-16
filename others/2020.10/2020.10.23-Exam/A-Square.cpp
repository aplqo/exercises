#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <random>
using namespace std;
const unsigned int maxn = 100000, maxa = 100000;
typedef pair<unsigned long long, unsigned long long> Hash;

unsigned int mindiv[maxa + 10], primes[maxa + 10], *pcur = primes;
unsigned int pid[maxa + 10];
unsigned long long pval[2][maxa + 10];
unsigned int a[maxn + 10];

static void eular(const unsigned int ma)
{
  for (unsigned long long i = 2; i <= ma; ++i) {
    if (!mindiv[i]) {
      pid[i] = pcur - primes;
      *(pcur++) = i;
      mindiv[i] = i;
    }
    for (const unsigned int* j = primes;
         j < pcur && *j * i <= ma && *j <= mindiv[i]; ++j)
      mindiv[*j * i] = *j;
  }
}
static void generateVal()
{
  ranlux24 rnd((random_device())());
  uniform_int_distribution<unsigned long long> dis(0, ULLONG_MAX);
  const unsigned int cn = pcur - primes;
  generate(pval[0], pval[0] + cn, bind(dis, ref(rnd)));
  generate(pval[1], pval[1] + cn, bind(dis, ref(rnd)));
}
Hash toHash(Hash old, unsigned long long v)
{
  while (v > 1) {
    const unsigned int p = pid[mindiv[v]];
    old.first ^= pval[0][p];
    old.second ^= pval[1][p];
    v /= mindiv[v];
  }
  return old;
}
unsigned long long count(const unsigned int n)
{
  Hash acc{0, 0};
  map<Hash, unsigned int> mp;
  unsigned long long ret = 0;
  for (const unsigned int* i = a; i < a + n; ++i) {
    acc = toHash(acc, *i);
    auto it = mp.find(acc);
    if (it == mp.end()) it = mp.insert({acc, 0}).first;
    ret += (it->second++);
    if (!acc.first && !acc.second) ++ret;
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
  copy_n(istream_iterator<unsigned int>(cin), n, a);
  eular(maxa);
  generateVal();
  cout << count(n) << "\n";
  return 0;
}