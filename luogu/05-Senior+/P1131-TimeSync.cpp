#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cctype>
#include <climits>
#include <cstdio>
using namespace std;
const unsigned int maxn = 500000;
constexpr unsigned long long inf = ULLONG_MAX / 2;

template <class T>
T read()
{
  char c = getchar();
  T ret = 0;
  while (!isdigit(c))
    c = getchar();
  while (isdigit(c)) {
    ret = ret * 10 + c - '0';
    c = getchar();
  }
  return ret;
}
struct edge {
  unsigned int to;
  unsigned long long val;
  edge* pre = nullptr;
} ed[maxn * 2 + 10];
edge* head[maxn + 1];
unsigned long long mdist[maxn + 1];

void addEdge(const unsigned int from, const unsigned int to,
             const unsigned long long val)
{
  static edge* cur = ed;
  cur->to = to;
  cur->val = val;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
unsigned long long dfs(const unsigned int x, const unsigned int f)
{
  unsigned long long ret = 0;
  for (edge* i = head[x]; i; i = i->pre) {
    if (i->to == f) continue;
    ret += dfs(i->to, x);
    mdist[i->to] += i->val;
    mdist[x] = max(mdist[x], mdist[i->to]);
  }
  for (edge* i = head[x]; i; i = i->pre)
    if (i->to != f) ret += mdist[x] - mdist[i->to];
  return ret;
}
int main()
{
  const unsigned int n = read<unsigned int>();
  const unsigned int s = read<unsigned int>();
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int s, t;
    unsigned long long v;
    s = read<unsigned int>();
    t = read<unsigned int>();
    v = read<unsigned long long>();
    addEdge(s, t, v);
    addEdge(t, s, v);
  }
  printf("%lld\n", dfs(s, 0));
  return 0;
}