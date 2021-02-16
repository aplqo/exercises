#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e4;

unsigned int dSum[maxn + 10];
int muSum[maxn + 10];

static void eular(const unsigned int n)
{
  static unsigned int mindiv[maxn + 10], primes[maxn + 10], *pcur = primes;
  static unsigned int d[maxn + 10], divCnt[maxn + 10];
  static int mu[maxn + 10];
  d[1] = mu[1] = 1;
  for (unsigned long long i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
      mu[i] = -1;
      d[i] = 2;
      divCnt[i] = 1;
    }
    for (const unsigned int* j = primes;
         j < pcur && *j < mindiv[i] && *j * i <= n; ++j) {
      const unsigned int v = *j * i;
      mindiv[v] = *j;
      mu[v] = -mu[i];
      d[v] = d[i] * 2;
      divCnt[v] = 1;
    }
    if (mindiv[i] * i <= n) {
      const unsigned int v = mindiv[i] * i;
      mindiv[v] = mindiv[i];
      d[v] = d[i] / (divCnt[i] + 1) * (divCnt[i] + 2);
      divCnt[v] = divCnt[i] + 1;
    }
  }
  for (unsigned int i = 1; i <= n; ++i) {
    dSum[i] = dSum[i - 1] + d[i];
    muSum[i] = muSum[i - 1] + mu[i];
  }
}

long long sum(const unsigned int n, const unsigned int m)
{
  const unsigned int lim = min(n, m);
  long long ret = 0;
  for (unsigned int i = 1; i <= lim;) {
    const unsigned int va = n / i, vb = m / i;
    const unsigned int nxt = min(n / va, m / vb);
    ret +=
        static_cast<long long>(muSum[nxt] - muSum[i - 1]) * dSum[va] * dSum[vb];
    i = nxt + 1;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  eular(maxn);
  for (unsigned int i = t; i; --i) {
    unsigned int n, m;
    cin >> n >> m;
    cout << sum(n, m) << "\n";
  }
  return 0;
}