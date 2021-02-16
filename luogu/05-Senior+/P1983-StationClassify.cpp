#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <queue>
using namespace std;
const int maxn = 1000, maxm = 1000;

struct vertex;
struct edge {
  vertex* to;
  unsigned int val = 0;
  edge* pre = nullptr;
} ed[maxn * maxn + 10];
edge* head[maxn + 10];
struct vertex {
  unsigned int ans = 0;
  unsigned int in = 0;
  edge* head = nullptr;
} ve[maxn + maxm + 10];
vertex* seq[maxn + maxm + 10];

inline void AddEdge(const unsigned int from, const unsigned int to,
                    unsigned int val)
{
  static edge* cur = ed;
  cur->to = ve + to;
  cur->val = val;
  cur->pre = ve[from].head;
  ve[from].head = cur;
  ++ve[to].in;
  ++cur;
}
inline void topological(const unsigned int n)
{
  queue<vertex*> q;
  for (vertex* i = ve; i < ve + n; ++i)
    if (!i->in) q.push(i);
  unsigned int pos = 0;
  while (!q.empty()) {
    vertex* cur = q.front();
    q.pop();
    seq[pos++] = cur;
    for (edge* i = cur->head; i; i = i->pre) {
      --(i->to->in);
      if (!(i->to->in)) q.push(i->to);
    }
  }
}
inline unsigned int dp(const unsigned int n)
{
  unsigned int ret = 0;
  for (unsigned int i = 0; i < n; ++i) {
    for (edge* j = seq[i]->head; j; j = j->pre)
      if (j->to->ans < seq[i]->ans + j->val) j->to->ans = seq[i]->ans + j->val;
  }
  for (unsigned int i = 0; i < n; ++i)
    if (seq[i]->ans > ret) ret = seq[i]->ans;
  return ret + 1;
}
int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n, m;
  cin >> n >> m;
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int s, cur, cnt = 0;
    cin >> s >> cur;
    --cur;
    for (unsigned int t = cur; t < n && cnt < s; ++t) {
      if (cur == t) {
        AddEdge(n + i, t, 0);
        ++cnt;
        if (cnt < s) {
          cin >> cur;
          --cur;
        }
      }
      else
        AddEdge(t, n + i, 1);
    }
  }
  topological(n + m);
  cout << dp(n + m) << endl;
  return 0;
}