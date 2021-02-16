#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <set>
using namespace std;
const unsigned int maxn = 1e5, maxb = 1e5;

pair<unsigned int, unsigned int> pos[maxn + 1];
struct Boot {
  unsigned int f, s;
  bool ans;
} bt[maxb + 1];
set<unsigned int> flr;
multiset<unsigned int, greater<unsigned int>> dis;

void add(const unsigned int pos)
{
  const static auto pre = [](set<unsigned int>::iterator it) { return --it; };
  const static auto nxt = [](set<unsigned int>::iterator it) { return ++it; };
  auto it = flr.insert(pos).first;
  const auto p = pre(it), n = nxt(it);
  dis.erase(dis.find(*n - *p));
  dis.insert(*it - *p);
  dis.insert(*n - *it);
}
void solve(const unsigned int n, const unsigned int m)
{
  static Boot* seq[maxb + 10];
  sort(pos + 2, pos + n);
  transform(bt, bt + m, seq, [](Boot& i) { return &i; });
  sort(seq, seq + m, [](const Boot* a, const Boot* b) { return a->f < b->f; });
  flr.insert(1);
  flr.insert(n);
  dis.insert(n - 1);
  auto it = pos + 2;
  for (unsigned int i = 0; i < m; ++i) {
    while (it < pos + n && it->first <= seq[i]->f)
      add((it++)->second);
    seq[i]->ans = *dis.begin() <= seq[i]->s;
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, b;
  cin >> n >> b;
  for (unsigned int i = 1; i <= n; ++i) {
    cin >> pos[i].first;
    pos[i].second = i;
  }
  for (Boot* i = bt; i < bt + b; ++i)
    cin >> i->f >> i->s;
  solve(n, b);
  for (Boot* i = bt; i < bt + b; ++i)
    cout << i->ans << "\n";
  return 0;
}