#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
using namespace std;
#define lowbit(x) ((x) & -(x))
const unsigned int maxn = 1024, maxm = 1000;

char gene[maxn][maxm + 10];
char root[maxm + 10];
unsigned int sol[maxn], lg[(1 << 25) + 1];

void init()
{
  for (unsigned int i = 1; i <= 25; ++i)
    lg[1 << i] = i;
}
unsigned int solve(const unsigned int n, const unsigned int pos)
{
  memset(sol, 0, sizeof(sol));
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i)
    sol[i] = 1 << (gene[i][pos] - 'A');
  for (unsigned int nxt = 1; nxt < n; nxt <<= 1) {
    const unsigned int step = nxt << 1;
    for (unsigned int i = 0; i + nxt < n; i += step) {
      const unsigned int cur = sol[i], ano = sol[i + nxt];
      if (cur & ano)
        sol[i] &= ano;
      else {
        ++ret;
        sol[i] |= ano;
      }
    }
  }
  root[pos] = 'A' + lg[lowbit(sol[0])];
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  init();
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < n; ++i)
    cin >> gene[i];
  unsigned int v = 0;
  for (unsigned int i = 0; i < m; ++i)
    v += solve(n, i);
  root[m] = '\0';
  cout << root << " " << v << endl;
  return 0;
}