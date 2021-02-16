// luogu: T123914
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;
const int maxn = 1e5, maxm = 1e5;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxm + 10];
unsigned int degree[maxn + 10], ans[maxn + 10], *en = ans;
edge* head[maxn + 10];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
  ++degree[to];
}
inline bool topological(unsigned int n)
{
  priority_queue<unsigned int, vector<unsigned int>, greater<unsigned int>> q;
  for (unsigned int i = 1; i <= n; ++i)
    if (!degree[i]) q.push(i);
  while (!q.empty()) {
    const unsigned int cur = q.top();
    q.pop();
    *(en++) = cur;
    for (const edge* i = head[cur]; i; i = i->pre) {
      --(degree[i->to]);
      if (!degree[i->to]) q.push(i->to);
    }
  }
  return en - ans == n;
}
int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    AddEdge(x, y);
  }
  if (topological(n))
    for (unsigned int* i = ans; i < en; ++i)
      cout << *i << " ";
  else
    cout << "OMG.";
  cout << endl;
  return 0;
}