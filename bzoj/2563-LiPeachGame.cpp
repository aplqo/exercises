#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const unsigned int maxn = 10000;

long long w[maxn + 1];

long long greedy(const unsigned int n)
{
  sort(w, w + n, greater<long long>());
  long long ret = 0;
  for (unsigned int i = 0; i < n; i += 2)
    ret += w[i] - w[i + 1];
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < n; ++i) {
    long long v;
    cin >> v;
    w[i] = v * 2;
  }
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    long long c;
    cin >> a >> b >> c;
    w[a - 1] += c;
    w[b - 1] += c;
  }
  cout << greedy(n) / 2 << endl;
  return 0;
}