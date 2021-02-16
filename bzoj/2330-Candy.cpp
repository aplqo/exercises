#ifdef APDEBUG
#include "debug_tools/checked.h"
#include "debug_tools/program.h"
using namespace apdebug::checked;
using num = CheckedInteger<long long>;
#else
typedef long long num;
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <limits>
#include <numeric>
using namespace std;
const int maxn = 100010, maxk = 100010 * 2 + maxn;

struct edge {
  edge() { pre = NULL; }
  int to;
  num val;
  edge* pre;
} ed[maxk];
edge* head[maxn];
int dep[maxn], seq[maxn];
bool vis[maxn];
num ans[maxn];
int n, k;

template <class T>
inline void read(T& val)
{
  val = 0;
  char t = getchar();
  while (!isdigit(t))
    t = getchar();
  while (isdigit(t)) {
    val = val * 10 + (t - '0');
    t = getchar();
  }
}
inline void AddEdge(int from, int to, num val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  cur->val = val;
  head[from] = cur;
  ++cur;
}
inline bool spfa(int s)
{
  deque<int> q;
  q.push_back(s);
  dep[s] = 1;
  while (!q.empty()) {
    int cur = q.front();
    q.pop_front();
    vis[cur] = false;
    for (edge* i = head[cur]; i; i = i->pre)
      if (ans[i->to] < ans[cur] + i->val) {
        ans[i->to] = ans[cur] + i->val;
        ++dep[i->to];
        if (dep[i->to] > n + 1) return false;
        if (!vis[i->to]) {
          if (!q.empty() && ans[i->to] > ans[q.front()])
            q.push_front(i->to);
          else
            q.push_back(i->to);
          vis[i->to] = true;
        }
      }
  }
  return true;
}
int main()
{
  read(n);
  read(k);
  for (int i = 0; i < k; ++i) {
    int x, a, b;
    read(x);
    read(a);
    read(b);
    switch (x) {
      case 1:
        AddEdge(a, b, 0);
        AddEdge(b, a, 0);
        break;
      case 2:
        AddEdge(a, b, 1);
        break;
      case 3:
        AddEdge(b, a, 0);
        break;
      case 4:
        AddEdge(b, a, 1);
        break;
      case 5:
        AddEdge(a, b, 0);
        break;
    }
  }
  for (int i = 1; i <= n; ++i)
    seq[i] = i;
  random_shuffle(seq + 1, seq + 1 + n);
  for (int i = 1; i <= n; ++i)
    AddEdge(0, seq[i], 1);
  ans[0] = 0;
  if (!spfa(0)) {
    cout << "-1" << endl;
    return 0;
  }
  cout << accumulate(ans + 1, ans + 1 + n, num(0)) << endl;
  return 0;
}