#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <utility>
using namespace std;
using num = unsigned long long;
constexpr num inf = numeric_limits<num>::max();
const int maxn = 1000;

enum { MAX = 0, MIN = 1 };
num mat[maxn + 10][maxn + 10];
num rrang[maxn + 10][maxn + 10][2],
    brang[maxn + 10][maxn + 10][2];  // row max/min, block max/min

template <class T>
class myque {
 public:
  num front() const { return q.front().second; }
  void push(unsigned int pos, num val)
  {
    while (!q.empty() && cmp(val, q.back().second))
      q.pop_back();
    q.push_back(make_pair(pos, val));
  }
  void pop(unsigned int pos)
  {
    while (!q.empty() && q.front().first < pos)
      q.pop_front();
  }

 private:
  T cmp;
  deque<pair<unsigned int, num>> q;
};

template <class T, unsigned int pos>
void col(const unsigned int a, const unsigned int b, const unsigned int n)
{
  for (unsigned int i = 0; i < a; ++i) {
    myque<T> q;
    for (unsigned int j = 0; j < n; ++j)
      q.push(j, mat[i][j]);
    rrang[i][0][pos] = q.front();
    for (unsigned int j = 0; j <= b - n; ++j) {
      q.pop(j);
      q.push(j + n - 1, mat[i][j + n - 1]);
      rrang[i][j][pos] = q.front();
    }
    for (unsigned int j = b - n; j < b; ++j) {
      q.pop(j);
      rrang[i][j][pos] = q.front();
    }
  }
}
template <class T, unsigned int pos>
void row(const unsigned int a, const unsigned int b, const unsigned int n)
{
  for (unsigned int i = 0; i <= b - n; ++i) {
    myque<T> q;
    for (unsigned int j = 0; j < n; ++j)
      q.push(j, rrang[j][i][pos]);
    brang[0][i][pos] = q.front();
    for (unsigned int j = 0; j <= a - n; ++j) {
      q.pop(j);
      q.push(j + n - 1, rrang[j + n - 1][i][pos]);
      brang[j][i][pos] = q.front();
    }
  }
}
num get_min(const unsigned int a, const unsigned int b, const unsigned int n)
{
  num ret = inf;
  for (unsigned int i = 0; i <= a - n; ++i)
    for (unsigned int j = 0; j <= b - n; ++j)
      ret = min(ret, brang[i][j][MAX] - brang[i][j][MIN]);
  return ret;
}
#ifdef APLOG
template <unsigned int pos>
void PrintTable(const num dat[maxn + 10][maxn + 10][2], const unsigned int a,
                const unsigned int b, const unsigned int n)
{
  for (unsigned int i = 0; i < a; ++i) {
    for (unsigned int j = 0; j < b; ++j) {
      clog.width(3);
      clog << dat[i][j][pos] << " ";
    }
    clog << endl;
  }
  clog << endl;
}
#endif

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int a, b, n;
  cin >> a >> b >> n;
  for (unsigned int i = 0; i < a; ++i)
    for (unsigned int j = 0; j < b; ++j)
      cin >> mat[i][j];
  col<less<num>, MIN>(a, b, n);
  col<greater<num>, MAX>(a, b, n);
  row<less<num>, MIN>(a, b, n);
  row<greater<num>, MAX>(a, b, n);
#ifdef APLOG
  PrintTable<MIN>(rrang, a, b, n);
  PrintTable<MAX>(rrang, a, b, n);
  PrintTable<MIN>(brang, a, b, n);
  PrintTable<MAX>(brang, a, b, n);
#endif
  cout << get_min(a, b, n) << endl;
  return 0;
}