/*
 *Luogu team T127828 math world
 */
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
const int maxn = 1e6;

bool sel[maxn + 10], abl[maxn + 10];
unsigned int to[maxn + 10], n;

unsigned int dfs(const unsigned int pos)
{
  if (pos > n) return 0;
  unsigned int ret = 0;
  {
    const bool old = abl[to[pos]];
    abl[to[pos]] = true;
    ret = dfs(pos + 1);
    abl[to[pos]] = old;
  }
  if (abl[pos]) {
    sel[pos] = true;
    ret = max(ret, dfs(pos + 1) + 1);
    sel[pos] = false;
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n;
  fill(abl + 1, abl + 1 + n, true);
  for (unsigned int i = 1; i <= n; ++i) {
    cin >> to[i];
    abl[to[i]] = false;
  }
  cout << dfs(1) << endl;
  return 0;
}