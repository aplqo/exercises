#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
const int maxn = 200, maxm = 200, maxk = 300;
constexpr unsigned long long mod = 1e9 + 7;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned long long ret = l.val + r.val;
    return Number{ret >= mod ? ret - mod : ret};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{(l.val * r.val) % mod};
  }
  inline void operator*=(const Number r) { val = (val * r.val) % mod; }

  unsigned long long val;
};
Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number x) { return quickPow(x, mod - 2); }

const struct {
  int x, y;
} step[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
struct Cycle {
};
enum class Direction {
  Down = 0,
  Up = 1,
  Right = 2,
  Left = 3,
  Unknown
} next[maxn + 1][maxm + 1];
enum class State { NotVisit, Finding, Visited } vis[maxn + 1][maxm + 1];
Number matrix[maxk + 1][maxk + 1];
int id[maxn + 1][maxm + 1], n, m, k;

Number gaussian(const int n)
{
  Number ret{1};
  for (int i = 0; i < n; ++i) {
    if (!matrix[i][i].val) {
      for (int j = i + 1; j < n; ++j)
        if (matrix[j][i].val) {
          std::swap_ranges(matrix[j] + i, matrix[j] + n, matrix[i] + i);
          ret.val = mod - ret.val;
          break;
        }
    }
    if (!matrix[i][i].val) return Number{0};
    const Number inv = inverse(matrix[i][i]);
    for (int j = i + 1; j < n; ++j)
      if (matrix[j][i].val) {
        const Number r = inv * matrix[j][i];
        for (int k = i; k < n; ++k)
          matrix[j][k] -= matrix[i][k] * r;
      }
  }
  return ret;
}
inline Number det(const int n)
{
  Number ret = gaussian(n - 1);
  for (int i = 0; i < n - 1; ++i)
    ret *= matrix[i][i];
  return ret;
}

int dfs(const int x, const int y)
{
  if (x < 0 || y < 0 || x >= n || y >= m) return k;
  switch (vis[x][y]) {
    case State::NotVisit: {
      vis[x][y] = State::Finding;
      const int d = static_cast<int>(next[x][y]);
      id[x][y] = dfs(x + step[d].x, y + step[d].y);
      vis[x][y] = State::Visited;
      return id[x][y];
    }
    case State::Finding:
      throw Cycle{};
    case State::Visited:
      return id[x][y];
  }
}
void read(const int n, const int m)
{
  k = 0;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j) {
      char c;
      std::cin >> c;
      switch (c) {
        case 'L':
          next[i][j] = Direction::Left;
          break;
        case 'R':
          next[i][j] = Direction::Right;
          break;
        case 'U':
          next[i][j] = Direction::Up;
          break;
        case 'D':
          next[i][j] = Direction::Down;
          break;
        case '.':
          next[i][j] = Direction::Unknown;
          id[i][j] = k++;
          vis[i][j] = State::Visited;
          break;
      }
    }
}
void buildMatrix(const int n, const int m)
{
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j)
      if (next[i][j] == Direction::Unknown) {
        const int cid = id[i][j];
        for (int k = 0; k < 4; ++k) {
          const int nxt = dfs(i + step[k].x, j + step[k].y);
          matrix[cid][nxt] -= Number{1};
          matrix[cid][cid] += Number{1};
        }
      }
      else
        dfs(i, j);
}
Number solve(const int n, const int m)
{
  read(n, m);
  try {
    buildMatrix(n, m);
  }
  catch (Cycle) {
    return Number{0};
  }
  return det(k + 1);
}
void clear()
{
  std::memset(matrix, 0, sizeof(matrix));
  std::memset(vis, 0, sizeof(vis));
  k = 0;
}

int main()
{
  std::ios::sync_with_stdio(false);
  int t;
  std::cin >> t;
  for (int i = t; i; --i) {
    std::cin >> n >> m;
    std::cout << solve(n, m).val << "\n";
    clear();
  }
  return 0;
}