#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;
const unsigned int maxn = 4e4, maxm = 1e6;

vector<unsigned int> gra[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to)
{
  gra[from].push_back(to);
  gra[to].push_back(from);
}
unsigned long long count(const unsigned int n, const unsigned int m)
{
  static bool vis[maxn + 10];
  const unsigned int cnt3 = n - gra[1].size() - gra[2].size() - 2;
  unsigned long long ret = 0;
  {
    static const auto cgraph = [](unsigned long long n) {
      return n * (n - 1) / 2;
    };
    ret = cgraph(gra[1].size()) + cgraph(gra[2].size()) + cgraph(cnt3);
  }
  vis[1] = vis[2] = true;
  for (unsigned int s = 1; s < 3; ++s) {
    for_each(gra[s].cbegin(), gra[s].cend(),
             [](unsigned int p) { vis[p] = true; });
    for (unsigned int i : gra[s]) {
      vis[i] = true;
      for (unsigned int j : gra[i])
        if (!vis[j]) {
          vis[j] = true;
          ret += gra[s].size();
        }
    }
  }
  ret += count(vis + 1, vis + 1 + n, false) * max(gra[1].size(), gra[2].size());
  return ret + gra[1].size() + gra[2].size() - m;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int u, v;
    cin >> u >> v;
    addEdge(u, v);
  }
  cout << count(n, m) << "\n";
  return 0;
}