#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <array>
#include <iostream>
#include <utility>
#include <vector>
using namespace std;
const unsigned int maxn = 50, maxm = 50, maxc = 100000;
typedef array<unsigned int, 2> Point;
typedef vector<Point> OperateSeq;

enum Dir { L = 0, R = 1, U = 2, D = 3, Null };
constexpr Dir rev[4] = {U, D, L, R};
constexpr Dir tab[26] = {Null, Null, Null, D,    Null, Null, Null, Null, Null,
                         Null, Null, L,    Null, Null, Null, Null, Null, R,
                         Null, Null, U,    Null, Null, Null, Null, Null};

template <bool isFlip>
class Matrix {
 public:
  void readMtrix(const unsigned int n, const unsigned int m)
  {
    for (unsigned int i = 1; i <= n; ++i)
      for (unsigned int j = 1; j <= m; ++j) {
        char ch;
        cin >> ch;
        if constexpr (isFlip)
          val[j][i] = rev[tab[ch - 'A']];
        else
          val[i][j] = tab[ch - 'A'];
      }
    this->n = n;
    this->m = m;
    if constexpr (isFlip) swap(this->n, this->m);
  }
  bool toHorizont()
  {
    for (unsigned int x = 1; x <= n; ++x)
      for (unsigned int y = 1; y <= m; ++y)
        if (val[x][y] == U && !change<L>(x, y)) return false;
    return true;
  }

  OperateSeq seq;

 private:
  void rotate(const unsigned int x, const unsigned int y)
  {
    if constexpr (isFlip)
      seq.push_back(Point{y, x});
    else
      seq.push_back(Point{x, y});
    if (val[x][y] == L) {
      val[x][y] = val[x][y + 1] = U;
      val[x + 1][y] = val[x + 1][y + 1] = D;
    }
    else {
      val[x + 1][y] = val[x][y] = L;
      val[x][y + 1] = val[x + 1][y + 1] = R;
    }
  }
  template <Dir typ>
  bool change(const unsigned int x, const unsigned int y)
  {
    if (val[x][y] == typ) return true;
    if (x >= n || y >= m) return false;
    if constexpr (typ == L) {
      if (val[x][y + 1] != U && !change<U>(x, y + 1)) return false;
    }
    else {
      if (val[x + 1][y] != L && !change<L>(x + 1, y)) return false;
    }
    rotate(x, y);
    return true;
  }

  unsigned int n, m;
  Dir val[maxn + 10][maxm + 10];
};
template <bool isFlip>
pair<bool, OperateSeq> constructSol(const unsigned int n, const unsigned int m)
{
  static Matrix<isFlip> stat, expect;
  stat.readMtrix(n, m);
  expect.readMtrix(n, m);
  if (!stat.toHorizont() || !expect.toHorizont())
    return make_pair(false, OperateSeq());
  OperateSeq ret = move(stat.seq);
  copy(expect.seq.crbegin(), expect.seq.crend(), back_inserter(ret));
  return make_pair(true, ret);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  const auto [suc, seq] =
      m & 0x01 ? constructSol<true>(n, m) : constructSol<false>(n, m);
  if (!suc)
    cout << "-1\n";
  else {
    cout << seq.size() << "\n";
    for (const auto [x, y] : seq)
      cout << x << " " << y << "\n";
  }
  return 0;
}