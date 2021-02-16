#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <numeric>
#include <utility>
using namespace std;
const unsigned int maxn = 1000;
using num = unsigned int;
constexpr num inf = numeric_limits<num>::max();

class mqueue {
 public:
  void push(const unsigned int p, const num val)
  {
    while (!q.empty() && q.back().second >= val)
      q.pop_back();
    q.emplace_back(p, val);
  }
  void pop(const unsigned int p)
  {
    while (!q.empty() && q.front().first < p)
      q.pop_front();
  }
  num front() const { return q.empty() ? inf : q.front().second; }
  void clear() { q.clear(); }

 private:
  deque<pair<unsigned int, num>> q;
};
num x[maxn + 1][maxn + 1], col[maxn + 1][maxn + 1];
unsigned int n, m;

void init(const unsigned int n, const unsigned int m)
{
  for (unsigned int i = 1; i <= m; ++i)
    for (unsigned int j = 1; j <= n; ++j)
      x[i][j] += x[i - 1][j] + x[i][j - 1] - x[i - 1][j - 1];
  for (unsigned int i = 0; i <= m; ++i)
    fill(col[i], col[i] + 1 + n, inf);
}
inline num sum(const unsigned int u, const unsigned int v,
               const unsigned int du, const unsigned int dv)
{
  return x[u][v] + x[u - du][v - dv] - x[u - du][v] - x[u][v - dv];
}
void calcCol(const unsigned int a, const unsigned int c, const unsigned int d)
{
  const auto ssum = bind(sum, placeholders::_1, placeholders::_2, c, d);
  const unsigned int k = a - c - 1;
  for (unsigned int i = d + 2; i <= n; ++i) {
    mqueue q;
    for (unsigned int j = c + 2; j < k; ++j) {
      q.push(j, ssum(j - 1, i - 1));
      col[j][i] = q.front();
    }
    for (unsigned int j = max(k, c + 2); j <= m; ++j) {
      q.pop(j + 1 - k);
      q.push(j, ssum(j - 1, i - 1));
      col[j][i] = q.front();
    }
  }
}
num calcRow(const unsigned int a, const unsigned int b, const unsigned int d)
{
  const auto rsum = bind(sum, placeholders::_1, placeholders::_2, a, b);
  num ret = 0;
  const unsigned int k = b - d - 1;
  for (unsigned int i = a; i <= m; ++i) {
    mqueue q;
    for (unsigned int j = d + 2; j < b; ++j)
      q.push(j, col[i][j]);
    for (unsigned int j = b; j <= n; ++j) {
      q.pop(j + 1 - k);
      q.push(j, col[i][j]);
      if (q.front() != inf) ret = max(ret, rsum(i, j) - q.front());
    }
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int a, b, c, d;
  cin >> m >> n >> a >> b >> c >> d;
  if (c + 2 > a || d + 2 > b) {
    cout << "0" << endl;
    return 0;
  }
  for (unsigned int i = 1; i <= m; ++i)
    copy_n(istream_iterator<num>(cin), n, x[i] + 1);
  init(n, m);
  calcCol(a, c, d);
  cout << calcRow(a, b, d) << endl;
  return 0;
}