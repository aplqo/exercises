#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
const int maxp = 10000, maxn = 500;

long long ans[maxn + 10][maxn + 10];
int primes[maxp + 10], *pcur = primes;

static void eular(const int n)
{
  static int mindiv[maxp + 10];
  for (int i = 2; i <= maxp && pcur < primes + n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
    }
    for (const int *j = primes; j < pcur && *j * i <= maxp && *j <= mindiv[i];
         ++j)
      mindiv[*j * i] = *j;
  }
}

template <int stepx, int stepy>
void fillDiagonal(const int n, const int v, int x, int y)
{
  while (x <= n && y <= n && x > 0 && y > 0) {
    ans[x][y] *= v;
    x += stepx;
    y += stepy;
  }
}
template <int stepx, int stepy>
void addFactor(const int n, const int v, const int ix, const int iy)
{
  ans[ix][iy] *= v;
  fillDiagonal<stepx, stepy>(n, v, ix + stepx, iy + stepy);
  fillDiagonal<(-stepx), -stepy>(n, v, ix - stepx, iy - stepy);
}
void constructDiagonal(const int n)
{
  eular(n * 2);
  int p = 0;
  for (int i = 1; i <= n; ++i)
    addFactor<1, -1>(n, primes[p++], i, i);
  const int cn = n & 0x01 ? n : n - 1;
  for (int i = 1; i <= cn; ++i)
    addFactor<1, 1>(n, primes[p++], i, cn + 1 - i);
}
void fillRest(const int n)
{
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j)
      if ((i + j) & 0x01)
        ans[i][j] = std::lcm(std::lcm(ans[i - 1][j], ans[i + 1][j]),
                             std::lcm(ans[i][j - 1], ans[i][j + 1])) +
                    1;
}

void construct(const int n)
{
  for (int i = 0; i <= n + 1; ++i)
    std::fill(ans[i], ans[i] + n + 2, 1);
  constructDiagonal(n);
  fillRest(n);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  if (n == 2) {
    std ::cout << R"(4 7
23 10

)";
    return 0;
  }
  construct(n);
  for (int i = 1; i <= n; ++i) {
    std::copy(ans[i] + 1, ans[i] + n + 1,
              std::ostream_iterator<long long>(std::cout, " "));
    std::cout.put('\n');
  }
  return 0;
}