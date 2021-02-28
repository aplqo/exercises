#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <vector>
const int maxn = 500000, maxl = 500000;
const int maxv = (maxn * 30 + maxl * 20) * 2,
          maxe = (maxl * 40 + maxn * 10) * 2;

int virtVertex;

template <bool v>
inline int var(const int x)
{
  return (x << 1) | v;
}
inline int notVar(const int x) { return x ^ 1; }

namespace TwoSAT {
struct Edge {
  int to;
  const Edge* pre;
} ed[maxe + 10];
const Edge* head[maxv + 10];
int scc[maxv + 10], sen;
int dfn[maxv + 10], low[maxv + 10], sta;

void addHalfEdge(const int from, const int to)
{
  static Edge* cur = ed;
  new (cur) Edge{to, head[from]};
  head[from] = cur++;
}
inline void implies(const int from, const int to)
{
  addHalfEdge(from, to);
  addHalfEdge(notVar(to), notVar(from));
}
void tarjan(const int x)
{
  static int stk[maxv + 10], *top = stk;
  *(top++) = x;
  dfn[x] = low[x] = ++sta;
  for (const Edge* i = head[x]; i; i = i->pre) {
    const int to = i->to;
    if (!dfn[to]) {
      tarjan(to);
      low[x] = std::min(low[x], low[to]);
    }
    else if (!scc[to])
      low[x] = std::min(low[x], dfn[to]);
  }
  if (dfn[x] == low[x]) {
    ++sen;
    while (*(top - 1) != x)
      scc[*(--top)] = sen;
    scc[*(--top)] = sen;
  }
}
bool check(const int v)
{
  for (int i = 0; i < v; ++i)
    if (!dfn[i]) tarjan(i);
  for (int i = 0; i < v; i += 2)
    if (scc[i] == scc[notVar(i)]) return false;
  return true;
}
}  // namespace TwoSAT

namespace Trie {
struct Node {
  int inVert, outVert;
  Node* child[2];
  std::vector<int> end;
} nodes[maxl * 2 + 1], *root = nodes, *cur = nodes + 1;

static void init()
{
  root->inVert = virtVertex++;
  root->outVert = virtVertex++;
}
void insert(const char str[], const size_t len, const int id)
{
  Node* ptr = root;
  for (const char* i = str; i < str + len; ++i) {
    const int v = *i - '0';
    if (!ptr->child[v]) {
      cur->inVert = virtVertex++;
      cur->outVert = virtVertex++;
      ptr->child[v] = cur++;
    }
    ptr = ptr->child[v];
  }
  ptr->end.push_back(id);
}
}  // namespace Trie

static void init(const int n)
{
  virtVertex = n;
  Trie::init();
}
void readinput(const int n)
{
  for (int i = 0; i < n; ++i) {
    static char str[maxl + 10];
    std::cin >> str;
    const size_t len = std::strlen(str);
    char* const p = std::find(str, str + len, '?');
    if (p == str + len) {
      TwoSAT::implies(var<true>(i), var<false>(i));
      Trie::insert(str, len, var<false>(i));
    }
    else {
      *p = '0';
      Trie::insert(str, len, var<false>(i));
      *p = '1';
      Trie::insert(str, len, var<true>(i));
    }
  }
}

inline void ifThen(const int x, const int y)
{
  TwoSAT::implies(var<true>(x), var<true>(y));
}
void linkSubtree(const Trie::Node* const x)
{
  if (x->child[0]) ifThen(x->outVert, x->child[0]->inVert);
  if (x->child[1]) ifThen(x->outVert, x->child[1]->inVert);
}
inline void linkEndpoint(const int ep, const int prev, const int next)
{
  TwoSAT::implies(var<true>(prev), notVar(ep));
  TwoSAT::implies(ep, var<true>(next));
}
template <int step>
void createVirtList(const Trie::Node* x, int dest[])
{
  const int n = x->end.size();
  const int begin = step == 1 ? 0 : n, end = step == 1 ? n : 0;
  dest[begin] = x->inVert;
  dest[end] = x->outVert;
  for (int i = begin + step; i != end; i += step)
    dest[i] = virtVertex++;
  for (int i = begin; i != end; i += step)
    ifThen(dest[i], dest[i + step]);
}
template <int step, int offset>
void linkEndList(const Trie::Node* x)
{
  static int list[maxn + 10];
  const std::vector<int>& end = x->end;
  const int n = end.size();
  createVirtList<step>(x, list);
  for (int i = 0; i < n; ++i)
    linkEndpoint(end[i], list[i + offset], list[i + offset + step]);
}
void buildGraph()
{
  for (Trie::Node* i = Trie::nodes; i < Trie::cur; ++i) {
    linkSubtree(i);
    switch (i->end.size()) {
      case 0:
        ifThen(i->inVert, i->outVert);
        break;
      case 1:
        linkEndpoint(i->end.front(), i->inVert, i->outVert);
        break;
      default:
        linkEndList<1, 0>(i);  // list to right
        linkEndList<(-1), 1>(i);
        break;
    }
  }
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  init(n);
  readinput(n);
  buildGraph();
  std::cout << (TwoSAT::check(virtVertex << 1) ? "YES" : "NO") << "\n";
  return 0;
}