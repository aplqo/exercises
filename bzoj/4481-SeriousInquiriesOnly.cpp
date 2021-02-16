#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 5e5;
typedef long double real;

real prob[maxn + 10];
vector<unsigned int> lst[maxn + 10];

#define lowbit(x) ((x) & -(x))
namespace FTree {
real val[maxn + 10];
void add(unsigned int pos, const unsigned int n, const real v)
{
  for (; pos <= n; pos += lowbit(pos))
    val[pos] += v;
}
real prefix(unsigned int pos)
{
  real ret = 0;
  for (; pos; pos -= lowbit(pos))
    ret += val[pos];
  return ret;
}
}  // namespace FTree
#undef lowbit

static void initProb(const unsigned int n, const real p)
{
  const real stp = 1 - p;
  real acc = stp;
  for (unsigned int i = 1; i <= n; ++i) {
    prob[i] = p / (1 - acc);
    acc *= stp;
  }
}
real solve(const unsigned int n, const real p)
{
  real ret = 0;
  for (unsigned int i = n; i > 0; --i) {
    const real base = prob[lst[i].size()], stp = 1 - p;
    {
      real acc = base;
      for (auto it = lst[i].cbegin(); it != lst[i].cend(); ++it) {
        ret += acc * FTree::prefix(*it - 1);
        acc *= stp;
      }
    }
    {
      real acc = base;
      for (auto it = lst[i].cbegin(); it != lst[i].cend(); ++it) {
        FTree::add(*it, n, acc);
        acc *= stp;
      }
    }
  }
  return ret;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  real p;
  cin >> n >> m >> setprecision(10) >> p;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    lst[a].push_back(b);
  }
  for (vector<unsigned int>* i = lst + 1; i < lst + 1 + n; ++i)
    sort(i->begin(), i->end());
  initProb(n, p);
  cout << fixed << setprecision(2) << solve(n, p) << "\n";
  return 0;
}