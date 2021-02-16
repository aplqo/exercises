#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <iostream>
#include <numeric>
#include <queue>
#include <random>
using namespace std;
using num_t = long long;
const int maxn = 100000, maxm = 100000;

struct edge {
  unsigned int to;
  num_t val;
  edge* pre = nullptr;
} ed[maxm * 2 + maxn * 3 + 10];
edge* head[maxn + 10];
bool vis[maxn + 10];
unsigned int cnt[maxn + 10];
num_t ans[maxn + 10];

template <class T>
inline void read(T& dat)
{
  dat = 0;
  char t = getchar();
  while (!isdigit(t))
    t = getchar();
  while (isdigit(t)) {
    dat = dat * 10 + t - '0';
    t = getchar();
  }
}
inline void AddEdge(const unsigned int from, const unsigned int to,
                    const num_t val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void init(const unsigned int n)
{
  static unsigned int tmp[maxn + 10];
  mt19937_64 rnd;
  rnd.seed(random_device()());
  iota(tmp, tmp + n, 1);
  shuffle(tmp, tmp + n, rnd);
  for (unsigned int i = 0; i < n; ++i)
    AddEdge(0, tmp[i], 1);
}
bool spfa(const unsigned int n)
{
  queue<unsigned int> q;
  q.push(0);
  while (!q.empty()) {
    unsigned int cur = q.front();
    q.pop();
    vis[cur] = false;
    for (edge* i = head[cur]; i; i = i->pre) {
      if (ans[i->to] < ans[cur] + i->val) {
        ans[i->to] = ans[cur] + i->val;
        ++cnt[i->to];
        if (cnt[i->to] > n) return false;
        if (!vis[i->to]) {
          vis[i->to] = true;
          q.push(i->to);
        }
      }
    }
  }
  return true;
}
int main()
{
  unsigned int n, m;
  read(n);
  read(m);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int o, a, b;
    read(o);
    read(a);
    read(b);
    switch (o) {
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
        AddEdge(b, a, 0);
        break;
    }
  }
  init(n);
  if (spfa(n))
    cout << accumulate(ans + 1, ans + 1 + n, 0) << endl;
  else
    cout << "-1" << endl;
  return 0;
}