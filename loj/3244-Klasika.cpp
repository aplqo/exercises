#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <set>
using namespace std;
const unsigned int maxn = 200000, maxl = 30;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxn + 10];
const Edge* head[maxn + 10];
struct Operate {
  enum Type { Add, Query } typ;
  unsigned int a, b;
} qry[maxn + 10];
unsigned int discover[maxn + 10], finish[maxn + 10], dis[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  static Edge* cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
}
namespace Trie {
struct Node {
  set<unsigned int> dfns;
  Node* chl[2];
};
Node root{{}, {}};

void insert(const unsigned int val, const unsigned int discover)
{
  Node* ptr = &root;
  for (unsigned int i = 1u << maxl; i; i >>= 1) {
    const bool b = val & i;
    if (!ptr->chl[b]) ptr->chl[b] = new Node{};
    ptr->dfns.insert(discover);
    ptr = ptr->chl[b];
  }
  ptr->dfns.insert(discover);
}
unsigned int maxXor(const unsigned int val, const unsigned int discover,
                    const unsigned int finish)
{
  static const auto existSubtree = [](const Node* ptr,
                                      const unsigned int discover,
                                      const unsigned int finish) {
    return ptr &&
           ptr->dfns.lower_bound(discover) != ptr->dfns.lower_bound(finish);
  };
  unsigned int ret = 0;
  Node* ptr = &root;
  for (unsigned int i = 1u << maxl; ptr && i; i >>= 1) {
    const bool b = !(val & i);
    if (existSubtree(ptr->chl[b], discover, finish)) {
      ret |= i;
      ptr = ptr->chl[b];
    }
    else
      ptr = ptr->chl[!b];
  }
  return ret;
}
}  // namespace Trie
void dfs(const unsigned int x, const unsigned int f)
{
  static unsigned int sta = 0;
  discover[x] = ++sta;
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      dis[i->to] = dis[x] ^ i->val;
      dfs(i->to, x);
    }
  finish[x] = ++sta;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int q;
  cin >> q;
  unsigned int cnt = 1;
  for (Operate* i = qry; i < qry + q; ++i) {
    char str[10];
    unsigned int ar1, ar2;
    cin >> str >> ar1 >> ar2;
    if (str[0] == 'A') {
      addEdge(ar1, ++cnt, ar2);
      *i = Operate{Operate::Type::Add, cnt};
    }
    else
      *i = Operate{Operate::Type::Query, ar1, ar2};
  }
  dfs(1, 0);
  Trie::insert(0, discover[1]);
  for (const Operate* i = qry; i < qry + q; ++i)
    if (i->typ == Operate::Type::Add)
      Trie::insert(dis[i->a], discover[i->a]);
    else
      cout << Trie::maxXor(dis[i->a], discover[i->b], finish[i->b]) << "\n";
  return 0;
}