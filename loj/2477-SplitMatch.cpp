#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
const int maxn = 200, maxm = 200, maxc = 10;
const int maxv = maxn * 2 + 2, maxe = maxn * maxc + maxn + maxm;

typedef int Index;
class Vector {
 public:
  inline void push_back(const int x) { *(ptr++) = x; }
  inline void clear() { ptr = dat; }
  inline const int* begin() const { return dat; }
  inline const int* end() const { return ptr; }
  inline bool empty() const { return ptr == dat; }

 private:
  int dat[maxc], *ptr = dat;
};
class Heap {
 public:
  inline void clear() { back = 0; }
  inline void push(const int x)
  {
    data[back++] = x;
    std::push_heap(data, data + back, pred);
  }
  inline void pop()
  {
    std::pop_heap(data, data + back, pred);
    --back;
  }
  inline int top() const { return data[0]; }
  inline bool empty() const { return !back; }

  struct VertexLess {
    inline bool operator()(const int a, const int b);
    class Graph* base;
  } pred;

 private:
  int data[maxv + 10];
  Index back;
};
int source, sink, n, m;
class Graph {
 public:
  void addEdge(const int from, const int to, const int flow);
  void addEdge(const int from, const Vector& to, const int flow);
  void init(const int n);
  inline void initMaxFlow() {}
  int updateMaxFlow(const int from);

  friend struct Heap::VertexLess;

 private:
  struct Edge {
    int from, to, flow;
    Index pre, rev;

    inline void push(Graph* const base, const int delt);
  } ed[maxe * 2 + 10];
  Index edgeEnd;
  struct Vertex {
    void relabel(Graph* const base);
    inline void push(Graph* const base);
    void pushRange(Graph* const base, const int from, const int to);

    Index head, cur;
    int h, exceed, vis;
    bool inHeap;
  } vert[maxv + 10];
  Heap heap;

  void bfs();

  static int visCounter;
} g[maxn + 1];
int Graph::visCounter;

int limit[maxm + 10], ideal[maxn + 10];
int admit[maxn + 10], minIncrease[maxn + 10];
Vector tar[maxn + 1][maxm + 1];

inline bool Heap::VertexLess::operator()(const int a, const int b)
{
  return base->vert[a].h < base->vert[b].h;
}
void Graph::addEdge(const int from, const int to, const int flow)
{
  new (ed + edgeEnd) Edge{from, to, flow, vert[from].head, edgeEnd + 1};
  vert[from].head = edgeEnd++;
  new (ed + edgeEnd) Edge{to, from, 0, vert[to].head, edgeEnd - 1};
  vert[to].head = edgeEnd++;
}
void Graph::addEdge(const int from, const Vector& to, const int flow)
{
  for (int i : to)
    addEdge(from, i, flow);
}
void Graph::init(const int n)
{
  heap.clear();
  edgeEnd = 1;
  std::memset(vert, 0, sizeof(vert[0]) * n);
}
int Graph::updateMaxFlow(const int from)
{
  const int old = vert[sink].exceed;
  bfs();
  if (vert[from].vis != visCounter) return 0;
  heap.pred.base = this;
  vert[from].exceed = 1;
  heap.push(from);
  while (!heap.empty()) {
    const int cur = heap.top();
    heap.pop();
    vert[cur].inHeap = false;
    vert[cur].push(this);
    while (vert[cur].exceed) {
      vert[cur].relabel(this);
      vert[cur].push(this);
    }
  }
  return vert[sink].exceed - old;
}
inline void Graph::Edge::push(Graph* const base, const int delt)
{
  flow -= delt;
  base->ed[rev].flow += delt;
  base->vert[from].exceed -= delt;
  base->vert[to].exceed += delt;
  if (to != source && to != sink && !base->vert[to].inHeap) {
    base->vert[to].inHeap = true;
    base->heap.push(to);
  }
}
void Graph::Vertex::relabel(Graph* const base)
{
  int mv = INT_MAX;
  for (int i = head; i; i = base->ed[i].pre) {
    Edge* const e = base->ed + i;
    if (e->flow) mv = std::min(mv, base->vert[e->to].h + 1);
  }
  h = mv;
}
inline void Graph::Vertex::push(Graph* const base)
{
  pushRange(base, cur, 0);
  if (exceed) pushRange(base, head, cur);
}
void Graph::Vertex::pushRange(Graph* const base, const int from, const int to)
{
  for (int i = from; i != to; i = base->ed[i].pre) {
    Edge* const e = base->ed + i;
    if (e->flow && base->vert[e->to].vis == Graph::visCounter &&
        base->vert[e->to].h + 1 == base->vert[e->from].h) {
      if (exceed < e->flow) {
        cur = e->pre;
        e->push(base, exceed);
        return;
      }
      else
        e->push(base, e->flow);
    }
  }
}
void Graph::bfs()
{
  std::queue<int> q;
  ++visCounter;
  q.push(sink);
  vert[sink].h = 0;
  vert[sink].vis = visCounter;
  while (!q.empty()) {
    const int cur = q.front();
    q.pop();
    for (int i = vert[cur].head; i; i = ed[i].pre)
      if (ed[ed[i].rev].flow && vert[ed[i].to].vis != visCounter) {
        vert[ed[i].to].vis = visCounter;
        vert[ed[i].to].h = vert[cur].h + 1;
        q.push(ed[i].to);
      }
  }
}

static void buildGraph(const int n, const int m)
{
  g[0].init(n + m + 2);
  for (int i = 1; i <= m; ++i)
    g[0].addEdge(n + i, sink, limit[i]);
  g[0].initMaxFlow();
}
void solveAdmission(const int n, const int m)
{
  for (int i = 1; i <= n; ++i) {
    admit[i] = m + 1;
    for (int j = 1; j <= m; ++j) {
      g[i] = g[i - 1];
      g[i].addEdge(source, i, 1);
      g[i].addEdge(i, tar[i][j], 1);
      if (g[i].updateMaxFlow(i)) {
        admit[i] = j;
        break;
      }
    }
  }
}
int binarySearch(const int x, const int ideal)
{
  int l = 0, r = x - 1;
  while (l <= r) {
    static Graph tmp;
    const int mid = (l + r) >> 1;
    tmp = g[x - mid - 1];
    tmp.addEdge(source, x, 1);
    for (int j = 1; j <= ideal; ++j)
      tmp.addEdge(x, tar[x][j], 1);
    if (tmp.updateMaxFlow(x))
      r = mid - 1;
    else
      l = mid + 1;
  }
  return l;
}
void solveIncrease(const int n)
{
  for (int i = 1; i <= n; ++i)
    minIncrease[i] = admit[i] <= ideal[i] ? 0 : binarySearch(i, ideal[i]);
}
void solve()
{
  std::cin >> n >> m;
  std::copy_n(std::istream_iterator<int>(std::cin), m, limit + 1);
  source = 0;
  sink = n + m + 1;
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j) {
      int c;
      std::cin >> c;
      if (c) tar[i][c].push_back(j + n);
    }
  std::copy_n(std::istream_iterator<int>(std::cin), n, ideal + 1);
  buildGraph(n, m);
  solveAdmission(n, m);
  solveIncrease(n);
  std::copy(admit + 1, admit + 1 + n,
            std::ostream_iterator<int>(std::cout, " "));
  std::cout.put('\n');
  std::copy(minIncrease + 1, minIncrease + 1 + n,
            std::ostream_iterator<int>(std::cout, " "));
  std::cout.put('\n');
}
void clear(const int n, const int m)
{
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
      tar[i][j].clear();
}

int main()
{
  std::ios::sync_with_stdio(false);
  int t, c;
  std::cin >> t >> c;
  for (int i = t; i; --i) {
    solve();
    clear(n, m);
  }
  return 0;
}
