#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <queue>
const int maxn = 500000;

int ans[maxn + 10][2], cnt;
template <class T, size_t maxSize>
struct Vector {
  inline void pushBack(const T x)
  {
    *(endPtr++) = x;
    ++size;
  }
  const int *begin() const { return data; }
  const int *end() const { return endPtr; }
  template <class Func>
  void foreachZip(unsigned int mask, Func fun)
  {
    for (T *i = data; i < endPtr; ++i, mask >>= 1)
      fun(*i, mask & 0x01u);
  }

  size_t size = 0;
  T data[maxSize], *endPtr = data;
};
struct Edge {
  int to;
  bool exist;
  Edge *prev, *next, *rev;

  inline void remove()
  {
    prev->next = next;
    next->prev = prev;
  }
  inline int from() const { return rev->to; }
  inline void setExist(const bool v) { exist = rev->exist = v; }
} ed[(maxn * 2 - 3) * 2], *cur = ed;
Edge head[maxn + 10];
struct Boundary {
  struct Node {
    int id, link;
  } prev, next;
  inline void remove(const int newLink);
} boundary[maxn + 10];
int graphDegree[maxn + 10], treeDegree[maxn + 10];

template <class T>
void addAnswer(const int from, const T x)
{
  ans[cnt][0] = from;
  ans[cnt][1] = x;
  ++treeDegree[from];
  ++treeDegree[x];
  ++cnt;
}
template <class T, class... Args>
inline void addAnswer(const int from, const T x, const Args... xs)
{
  addAnswer(from, x);
  addAnswer(from, xs...);
}

inline void Boundary::remove(const int newLink)
{
  boundary[prev.id].next = Node{next.id, newLink};
  boundary[next.id].prev = Node{prev.id, newLink};
}
Edge *addHalfEdge(const int from, const int to)
{
  new (cur) Edge{to, true, head[from].prev, &head[from], nullptr};
  cur->prev->next = cur->next->prev = cur;
  return cur++;
}
void addEdge(const int from, const int to)
{
  Edge *ea = addHalfEdge(from, to), *eb = addHalfEdge(to, from);
  ea->rev = eb;
  eb->rev = ea;
  ++graphDegree[from];
  ++graphDegree[to];
}
inline void removeEdge(Edge *e, std::queue<int> &q)
{
  --graphDegree[e->to];
  --graphDegree[e->from()];
  if (graphDegree[e->to] == 2) q.push(e->to);
  e->remove();
  e->rev->remove();
}

static void init(const int n)
{
  for (Edge *i = head + 1; i < head + n + 1; ++i)
    i->prev = i->next = i;
  for (int i = 1; i < n; ++i) {
    addEdge(i, i + 1);
    boundary[i] = Boundary{{i - 1}, {i + 1}};
  }
  boundary[1].prev.id = n;
  addEdge(n, 1);
  boundary[n] = Boundary{{n - 1}, {1}};
}
void reduceVertex(const int n)
{
  std::queue<int> q;
  int last = n;
  for (int i = 1; i <= n; ++i)
    if (graphDegree[i] == 2) q.push(i);
  while (last > 4) {
    const int cur = q.front();
    q.pop();
    --last;

    Boundary &bnd = boundary[cur];
    if (bnd.prev.link && bnd.next.link) {
      addAnswer(cur, bnd.prev.link, bnd.next.link);
      bnd.remove(cur);
    }
    else if (bnd.prev.link || bnd.next.link) {
      Boundary::Node &p = bnd.prev.link ? bnd.prev : bnd.next;
      addAnswer(p.id, p.link, cur);
      bnd.remove(0);
    }
    else
      bnd.remove(cur);

    removeEdge(head[cur].prev, q);
    removeEdge(head[cur].next, q);
  }
}
namespace BruteForce {
struct Link {
  int x, target[2];
};
Vector<Link, 5> link;
Vector<Edge *, 5> edge;
Vector<int, 4> vertex;
int vis[maxn + 10], visId;
int extraDegree[maxn + 10];

void findRemain(const int n)
{
  for (int i = 1; i <= n; ++i)
    if (graphDegree[i]) {
      vertex.pushBack(i);
      for (Edge *j = head[i].next; j != &head[i]; j = j->next)
        if (j > j->rev) edge.pushBack(j);
      const Boundary::Node &prev = boundary[i].prev;
      if (prev.link) link.pushBack(Link{prev.link, {prev.id, i}});
    }
}

bool dfs(const int x, const int f)
{
  vis[x] = visId;
  for (Edge *i = head[x].next; i != &head[x]; i = i->next)
    if (i->to != f && i->exist && (vis[i->to] == visId || !dfs(i->to, x)))
      return false;
  return true;
}
template <class T, size_t v, class Pred, class Func>
bool enumerateMask(Vector<T, v> &s, Pred pred, Func fun)
{
  const unsigned int lim = 1u << s.size;
  for (unsigned int i = 0; i < lim; ++i)
    if (pred(i)) {
      s.foreachZip(i, fun);
      return true;
    }
  return false;
}

bool checkDegree(unsigned int msk)
{
  static unsigned int degree[maxn + 10];
  for (int p : vertex)
    degree[p] = extraDegree[p];
  link.foreachZip(msk, [](Link &i, bool p) { ++degree[i.target[p]]; });
  for (int p : vertex)
    if (degree[p] == 2) return false;
  return true;
}
bool enumerateLink()
{
  return enumerateMask(link, checkDegree,
                       [](Link &i, bool p) { addAnswer(i.x, i.target[p]); });
}

inline void setEdge(Edge *e, bool p)
{
  e->setExist(p);
  if (p) {
    ++extraDegree[e->to];
    ++extraDegree[e->from()];
  }
}
bool checkSpanningTree(const unsigned int msk)
{
  if (__builtin_popcount(msk) != vertex.size - 1) return false;
  for (int p : vertex)
    extraDegree[p] = treeDegree[p];
  edge.foreachZip(msk, setEdge);
  ++visId;
  return dfs(vertex.data[0], 0) && enumerateLink();
}
void enumerateSpanTree()
{
  enumerateMask(edge, checkSpanningTree, [](Edge *p, bool v) {
    if (v) addAnswer(p->to, p->from());
  });
}

void enumerate(const int n)
{
  findRemain(n);
  enumerateSpanTree();
}
}  // namespace BruteForce

void solve(const int n)
{
  reduceVertex(n);
  BruteForce::enumerate(n);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n;
  std::cin >> n;
  if (n == 3) {
    std::cout << "-1\n";
    return 0;
  }
  init(n);
  for (int i = n - 3; i; --i) {
    int a, b;
    std::cin >> a >> b;
    addEdge(a, b);
  }
  solve(n);
  for (int i = 0; i < cnt; ++i)
    std::cout << ans[i][0] << " " << ans[i][1] << "\n";
  return 0;
}
