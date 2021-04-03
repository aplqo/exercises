#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxN = 5e5, maxC = 22;
const unsigned int maxState = 1u << maxC;

template <class T, size_t maxSize>
class Array {
 public:
  void clear()
  {
    for (const int* i = stack; i < top; ++i)
      data[*i] = 0;
    top = stack;
  }
  void updateMax(const int x, const T val)
  {
    if (val > data[x]) {
      if (!data[x]) *(top++) = x;
      data[x] = val;
    }
  }

  const T* const dataPtr = data;

 private:
  int stack[maxSize], *top = stack;
  T data[maxSize];
};
struct Edge {
  int to, ch;
  const Edge* pre;
} ed[maxN + 10];
const Edge* head[maxN + 10];
Array<int, maxState> maxDepth;
unsigned int mask[maxN + 10];
int son[maxN + 10], dep[maxN + 10], answer[maxN + 10];

inline void addEdge(const int from, const int to, const char ch)
{
  static Edge* cur = ed;
  new (cur) Edge{to, ch - 'a', head[from]};
  head[from] = cur++;
}
int dfsSon(const int x, const int f)
{
  int ms = 0, ret = 1;
  dep[x] = dep[f] + 1;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const int to = i->to;
    mask[to] = mask[x] ^ (1u << i->ch);
    const int sz = dfsSon(to, x);
    ret += sz;
    if (sz > ms) {
      ms = sz;
      son[x] = to;
    }
  }
  return ret;
}

int findMax(const int dep, const unsigned int mask)
{
  int ret = 0;
  if (maxDepth.dataPtr[mask]) ret = maxDepth.dataPtr[mask] + dep;
  for (unsigned int i = 1; i < maxState; i <<= 1)
    if (maxDepth.dataPtr[mask ^ i])
      ret = std::max(ret, maxDepth.dataPtr[mask ^ i] + dep);
  return ret;
}
void addSubtree(const int x)
{
  maxDepth.updateMax(mask[x], dep[x]);
  for (const Edge* i = head[x]; i; i = i->pre)
    addSubtree(i->to);
}
int dfsPath(const int x)
{
  int ret = findMax(dep[x], mask[x]);
  for (const Edge* i = head[x]; i; i = i->pre)
    ret = std::max(ret, dfsPath(i->to));
  return ret;
}
int dfs(const int x)
{
  int ret = 0;
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != son[x]) {
      ret = std::max(ret, dfs(i->to));
      maxDepth.clear();
    }
  if (son[x]) ret = std::max(ret, dfs(son[x]));
  maxDepth.updateMax(mask[x], dep[x]);
  int val = findMax(dep[x], mask[x]);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != son[x]) {
      val = std::max(val, dfsPath(i->to));
      addSubtree(i->to);
    }
  return answer[x] = std::max(ret, val - 2 * dep[x]);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  for (int i = 2; i <= n; ++i) {
    int p;
    char c;
    std::cin >> p >> c;
    addEdge(p, i, c);
  }
  dfsSon(1, 0);
  dfs(1);
  std::copy(answer + 1, answer + n + 1,
            std::ostream_iterator<int>(std::cout, " "));
  std::cout.put('\n');
  return 0;
}