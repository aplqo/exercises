#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 14, maxt = 3, maxv = 10;

const char str[] = {'m', 'p', 's'};
int cnt[maxv][maxt], orig[maxv][maxt];
unsigned int sum;

void readCard(const unsigned int n)
{
  memset(orig, 0, sizeof(orig));
  for (unsigned int i = 0; i < n; ++i) {
    static char str[4];
    cin >> str;
    switch (str[1]) {
      case 'm':
        ++orig[str[0] - '0'][0];
        break;
      case 'p':
        ++orig[str[0] - '0'][1];
        break;
      case 's':
        ++orig[str[0] - '0'][2];
        break;
    }
  }
}
template <int delt>
inline void changeKe(const unsigned int num, const unsigned int typ)
{
  cnt[num][typ] += delt;
  sum += delt;
}
template <int delt>
inline void changeShun(const unsigned int pos, const unsigned int typ)
{
  for (unsigned int i = 0; i < 3; ++i)
    cnt[pos + i][typ] += delt;
  sum += delt * 3;
}

bool dfs(const unsigned int num)
{
  if (num >= maxv || sum < 3) return !sum;
  if (max({cnt[num][0], cnt[num][1], cnt[num][2]}) == 0) return dfs(num + 1);
  for (unsigned int i = 0; i < maxt; ++i) {
    if (cnt[num][i] >= 3) {
      changeKe<(-3)>(num, i);
      if (dfs(num)) return true;
      changeKe<3>(num, i);
    }
    if (num + 2 < maxv &&
        min({cnt[num][i], cnt[num + 1][i], cnt[num + 2][i]}) > 0) {
      changeShun<(-1)>(num, i);
      if (dfs(num)) return true;
      changeShun<1>(num, i);
    }
  }
  return false;
}
static void initDfs()
{
  memcpy(cnt, orig, sizeof(orig));
  sum = 14;
}
bool check()
{
  for (unsigned int i = 0; i < maxt; ++i)
    for (unsigned int j = 1; j < maxv; ++j)
      if (cnt[j][i] > 1) {
        changeKe<(-2)>(j, i);
        if (dfs(1)) return true;
        changeKe<(2)>(j, i);
      }
  return false;
}
void findSolution()
{
  readCard(13);
  for (unsigned int j = 0; j < maxt; ++j)
    for (unsigned int i = 1; i < maxv; ++i) {
      if (orig[i][j] == 4) continue;
      initDfs();
      ++cnt[i][j];
      if (check()) cout << i << str[j] << " ";
    }
  cout.put('\n');
}
void checkSolution()
{
  readCard(14);
  initDfs();
  cout << (check() ? '1' : '0') << "\n";
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t, n;
  cin >> n >> t;
  void (*const fun)(void) = n == 14 ? checkSolution : findSolution;
  for (unsigned int i = 0; i < t; ++i)
    fun();
  return 0;
}