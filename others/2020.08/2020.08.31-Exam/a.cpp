#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
using namespace std;
const unsigned int maxn = 2e5;

multiset<int> pos, neg;
int a[maxn + 1], acc;
long long sum;

inline void insert(const int v)
{
  if (v + acc < 0)
    neg.insert(v);
  else
    pos.insert(v);
  sum += abs(v + acc);
}
inline void remove(const int v)
{
  if (v + acc < 0)
    neg.erase(neg.find(v));
  else
    pos.erase(pos.find(v));
  sum -= abs(v + acc);
}
void increase()
{
  multiset<int>::iterator it = neg.upper_bound(-acc);
  pos.insert(it, neg.end());
  neg.erase(it, neg.end());
  sum = sum + pos.size() - neg.size();
}
void rotate(const unsigned int n, const unsigned int i)
{
  ++acc;
  increase();
  remove(a[i]);
  a[i] -= n;
  insert(a[i]);
}
long long findMin(const unsigned int n)
{
  long long ret = sum;
  for (unsigned int i = 1; i < n; ++i) {
    rotate(n, i);
    ret = min(ret, sum);
  }
  return ret;
}

int main()
{
#if !defined APTEST && !defined APDEBUG
  ios_base::sync_with_stdio(false);
  ifstream in("a.in");
  ofstream out("a.out");
#else
  istream& in = cin;
  ostream& out = cout;
#endif
  unsigned int n;
  in >> n;
  for (int i = 1; i <= n; ++i) {
    in >> a[i];
    a[i] -= i;
    insert(a[i]);
  }
  out << findMin(n) << "\n";
  return 0;
}