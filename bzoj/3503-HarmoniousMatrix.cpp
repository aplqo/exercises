#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <bitset>
#include <iostream>
#include <iterator>
#include <numeric>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 40;
using Point = pair<int, int>;
using nxtILst = vector<int>;

int n, m;
bitset<maxn * maxn + 1> equ[maxn * maxn + 1];
nxtILst nxt[maxn * maxn + 1];
bool ans[maxn + 1][maxn + 1];

inline int id(Point p) { return p.first * m + p.second; }
nxtILst nextId(const Point p)
{
  static const int dx[] = {0, 0, -1, 1}, dy[] = {1, -1, 0, 0};
  nxtILst ret;
  ret.reserve(4);
  for (unsigned int i = 0; i < 4; ++i) {
    const int nx = p.first + dx[i], ny = p.second + dy[i];
    if (nx >= 0 && nx < n && ny >= 0 && ny < m)
      ret.emplace_back(id(Point(nx, ny)));
  }
  return ret;
}
static void init(const int n, const int m)
{
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      nxt[id(Point(i, j))] = nextId(Point(i, j));
}
static void buildEquation(const int n, const int m)
{
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j) {
      const int cp = id(Point(i, j));
      equ[cp].set(cp);
      for (int nid : nxt[cp])
        equ[cp].set(nid);
    }
}
void gaussian()
{
  const int siz = n * m;
  for (int i = 0; i < siz; ++i) {
    if (!equ[i][i]) {
      for (int j = i + 1; j < siz; ++j)
        if (equ[j][i]) {
          swap(equ[j], equ[i]);
          break;
        }
    }
    if (!equ[i][i]) continue;
    for (int j = i + 1; j < siz; ++j)
      if (equ[j][i]) equ[j] ^= equ[i];
  }
  for (int i = siz - 1; i >= 0; --i) {
    if (!equ[i][i]) equ[i][siz] = 1;
    for (int j = 0; j < i; ++j)
      if (equ[j][i]) equ[j] ^= equ[i];
  }
}
void buildAns(const int n, const int m)
{
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      ans[i][j] = equ[id(Point(i, j))].test(n * m);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin >> n >> m;
  init(n, m);
  buildEquation(n, m);
  gaussian();
  buildAns(n, m);
  for (unsigned int i = 0; i < n; ++i) {
    copy(ans[i], ans[i] + m, ostream_iterator<int>(cout, " "));
    cout << endl;
  }
  return 0;
}