#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>

const int maxna = 2e5, maxnb = 2e5, maxs = 2e5, maxm = 2e5, maxc = 27,
          maxt = 100;
const int maxv = maxs * 3 + maxna + maxnb,
          maxe = (maxs + 10) * 3 + maxnb + maxm + maxna;

typedef long long Distance;
template <class T, int maxSize>
class Vector {
 public:
  inline void clear() { beginPtr = dat + maxSize; }
  inline void push_front(T x) { *(--beginPtr) = x; }
  inline T front() const { return *beginPtr; }
  inline const T *begin() const { return beginPtr; }
  inline const T *end() const { return dat + maxSize; }

 private:
  T dat[maxSize], *beginPtr;
};
struct Substr {
  int len, vertex;
};
Substr typeA[maxna + 10], typeB[maxnb + 10];

namespace DAG {
struct Edge {
  int to, val;
  const Edge *pre;
} ed[maxe + 1], *cur = ed;
const Edge *head[maxv + 10];
int degree[maxv + 10];

inline void addEdge(const int from, const int to, const int val)
{
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  ++degree[to];
}

Distance topological(const int n, const int allowCnt)
{
  static Distance dis[maxv + 10];
  std::queue<int> q;
  std::memset(dis, 0, sizeof(dis[0]) * n);
  for (int i = 0; i < n; ++i)
    if (!degree[i]) q.push(i);
  int cnt = 0;
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    ++cnt;
    for (const Edge *i = head[cur]; i; i = i->pre) {
      const int to = i->to;
      --degree[to];
      dis[to] = std::max(dis[to], dis[cur] + i->val);
      if (!degree[to]) q.push(to);
    }
  }
  return cnt < n ? -1 : *std::max_element(dis, dis + allowCnt);
}
void clear(const int n)
{
  std::memset(degree, 0, sizeof(degree[0]) * n);
  std::memset(head, 0, sizeof(head[0]) * n);
  cur = ed;
}
}  // namespace DAG

namespace SuffixTree {
struct Node {
  int len, id;
  Node *link, *next[maxc];
  std::vector<Substr *> *substr;
  std::vector<Node *> child;
} nodes[maxs * 3 + 10], *root = nodes, *cur = nodes + 1;
Node *prefix[maxs + 10];

Node *insertChar(Node *const last, const int ch)
{
  Node *const ptr = cur++, *from = last;
  ptr->len = last->len + 1;
  while (from && !from->next[ch]) {
    from->next[ch] = ptr;
    from = from->link;
  }
  if (!from)
    ptr->link = root;
  else {
    Node *const fNxt = from->next[ch];
    if (fNxt->len == from->len + 1)
      ptr->link = fNxt;
    else {
      Node *const cloned = cur++;
      std::memcpy(cloned->next, fNxt->next, sizeof(cloned->next));
      cloned->link = fNxt->link;
      cloned->len = from->len + 1;
      for (Node *i = from; i && i->next[ch] == fNxt; i = i->link)
        i->next[ch] = cloned;
      fNxt->link = ptr->link = cloned;
    }
  }
  return ptr;
}
static void initChild()
{
  for (Node *i = nodes + 1; i < cur; ++i)
    i->link->child.push_back(i);
}
void clear()
{
  for (Node *i = nodes; i < cur; ++i) {
    i->substr = nullptr;
    i->child.clear();
    std::memset(i->next, 0, sizeof(i->next));
  }
  cur = nodes + 1;
}
void build(char str[], const size_t s)
{
  prefix[s + 1] = root;
  str[s] = 'a' + 26;
  for (int i = s; i >= 0; --i)
    prefix[i] = insertChar(prefix[i + 1], str[i] - 'a');
  initChild();
}
}  // namespace SuffixTree

namespace GraphBuilder {
typedef Vector<int, maxs> LengthList;
std::vector<Substr *> bucket[maxs + 10], substr[maxs + 10];
std::priority_queue<int> heap;
int virtVertex;

inline void addSubstr(std::vector<Substr *> *lst)
{
  if (!lst) return;
  for (auto i : *lst) {
    if (bucket[i->len].empty()) heap.push(i->len);
    bucket[i->len].push_back(i);
  }
  lst->clear();
}
inline void createVertex(const int *begin, const int *end, int dest[])
{
  for (const int *i = begin; i != end; ++i)
    dest[*i] = ++virtVertex;
}
inline void linkVertex(const int first, const LengthList &list,
                       const int dest[])
{
  const int *const end = list.end();
  DAG::addEdge(first, dest[*list.begin()], 0);
  for (const int *i = list.begin() + 1; i != end; ++i)
    DAG::addEdge(dest[*(i - 1)], dest[*i], 0);
}
inline void extractLength(SuffixTree::Node *const x, LengthList &list)
{
  list.clear();
  list.push_front(x->len);
  while (!heap.empty() && heap.top() > x->link->len) {
    const int c = heap.top();
    heap.pop();
    if (c != list.front()) list.push_front(c);
  }
}
inline void splitVertex(SuffixTree::Node *const x, LengthList &len, int dest[])
{
  extractLength(x, len);
  createVertex(len.begin(), len.end() - 1, dest);
  dest[x->len] = x->id;
  linkVertex(x->link->id, len, dest);
}
inline void setLocated(const LengthList &len, const int vert[])
{
  for (int l : len) {
    for (auto p : bucket[l])
      p->vertex = vert[l];
    bucket[l].clear();
  }
}
template <bool isRoot>
void dfs(SuffixTree::Node *const x)
{
  x->id = ++virtVertex;
  for (auto j : x->child)
    dfs<false>(j);
  if constexpr (!isRoot) {
    static int splited[maxs + 10];
    static LengthList length;
    addSubstr(x->substr);
    splitVertex(x, length, splited);
    setLocated(length, splited);
  }
}
int build(char str[], const size_t s, const int na)
{
  SuffixTree::build(str, s);
  virtVertex = na - 1;
  for (size_t i = 0; i < s; ++i)
    SuffixTree::prefix[i]->substr = substr + i;
  dfs<true>(SuffixTree::root);
  for (int i = 0; i < na; ++i)
    DAG::addEdge(typeA[i].vertex, i, typeA[i].len);
  return virtVertex + 1;
}
inline void addRelation(const int a, const int b)
{
  DAG::addEdge(a, typeB[b].vertex, 0);
}
}  // namespace GraphBuilder

void clear(const int n)
{
  DAG::clear(n);
  SuffixTree::clear();
}
inline int readSubstr(Substr *const begin)
{
  int n;
  std::cin >> n;
  for (Substr *i = begin; i < begin + n; ++i) {
    int l, r;
    std::cin >> l >> r;
    i->len = r + 1 - l;
    GraphBuilder::substr[l - 1].push_back(i);
  }
  return n;
}
void solve()
{
  static char str[maxs + 10];
  int m;
  std::cin >> str;
  const size_t s = std::strlen(str);
  const int na = readSubstr(typeA);
  readSubstr(typeB);
  const int vert = GraphBuilder::build(str, s, na);
  std::cin >> m;
  for (int i = m; i; --i) {
    int x, y;
    std::cin >> x >> y;
    GraphBuilder::addRelation(x - 1, y - 1);
  }
  std::cout << DAG::topological(vert, na) << "\n";
  clear(vert);
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int t;
  std::cin >> t;
  for (int i = t; i; --i)
    solve();
  return 0;
}