#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cctype>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <sstream>
#include <unordered_map>
using namespace std;
const unsigned int maxn = 1e4, maxm = 1e5, maxt = 32;

char inbuf[maxt * 40 + maxm * maxt * 40 + 1000],
    *inpos = inbuf, outbuf[maxt * maxn * 2 * 20 + 1000], *outpos = outbuf;

unsigned int readUInt()
{
  unsigned int ret = 0;
  char c = *(inpos++);
  while (!isdigit(c))
    c = *(inpos++);
  while (isdigit(c)) {
    ret = ret * 10 + (c - '0');
    c = *(inpos++);
  }
  return ret;
}
template <class... Args>
void writeUInt(const char* fmt, Args... args)
{
  outpos += sprintf(outpos, fmt, args...);
}

class Graph {
 public:
  struct Edge {
    unsigned int to;
    Edge *prev, *next, *rev;

    inline void remove()
    {
      prev->next = next;
      next->prev = prev;
    }
  };
  Edge head[maxn + 10];
  unsigned int degree[maxn + 10];
  void init(const unsigned int n)
  {
    cur = nodes;
    memset(degree, 0, sizeof(degree));
    for (Edge* i = head; i < head + n + 1; ++i)
      i->prev = i->next = i;
  }
  inline void addEdge(const unsigned int from, const unsigned int to)
  {
    Edge *const e1 = addHalf(from, to), *const e2 = addHalf(to, from);
    e1->rev = e2;
    e2->rev = e1;
    ++degree[from];
    ++degree[to];
  }

 private:
  inline Edge* addHalf(const unsigned int from, const unsigned int to)
  {
    new (cur) Edge{to, head[from].prev, &head[from], nullptr};
    head[from].prev->next = cur;
    head[from].prev = cur;
    return cur++;
  }
  Edge nodes[maxm * 2 + 10], *cur = nodes;
} g1, g2;
struct ValFrom {
  unsigned int from, val;
  inline bool operator<(const ValFrom& r) const
  {
    return val != r.val ? val < r.val : from < r.from;
  }
};
class Set : set<ValFrom>, unordered_map<unsigned int, set<ValFrom>::iterator> {
 private:
  using set_t = set<ValFrom>;
  using map_t = unordered_map<unsigned int, set_t::iterator>;

 public:
  void assign(const unsigned int from, const unsigned int val)
  {
    auto it = map_t::find(from);
    set_t::erase(it->second);
    it->second = set_t::insert(ValFrom{from, val}).first;
  }
  inline ValFrom top() const { return *set_t::begin(); }
  inline void erase(const unsigned int from)
  {
    auto it = map_t::find(from);
    set_t::erase(it->second);
    map_t::erase(it);
  }
  inline void insert(const unsigned int from, const unsigned int val)
  {
    map_t::insert({from, set_t::insert(ValFrom{from, val}).first});
  }
  inline void init(const unsigned int n) { map_t::reserve(n); }
  using set_t::begin;
  using set_t::empty;
  using set_t::end;
};

inline void init(const unsigned int n, const Graph& g, Set& h)
{
  h.init(n);
  for (unsigned int i = 1; i <= n; ++i)
    h.insert(i, g.degree[i]);
}
inline void removeVertex(const unsigned int v, Graph& g, Set& h)
{
  for (auto j = g.head[v].next; j != &g.head[v]; j = j->next) {
    j->rev->remove();
    h.assign(j->to, --g.degree[j->to]);
  }
  h.erase(v);
}
void constructVertexSet(const unsigned int n, Graph& g)
{
  static char buf[maxn * 20 + 10];
  char* pos = buf;
  Set s;
  init(n, g, s);
  unsigned int cnt = 0;
  while (!s.empty()) {
    const ValFrom v = s.top();
    removeVertex(v.from, g, s);
    for (auto i = g.head[v.from].next; i != &g.head[v.from]; i = i->next)
      removeVertex(i->to, g, s);
    ++cnt;
    pos += sprintf(pos, "%u ", v.from);
  }
  *pos = '\0';
  writeUInt("%u %s\n", cnt, buf);
}
void constructSubgraph(const unsigned int n, Graph& g)
{
  Set s;
  init(n, g, s);
  unsigned int cnt = n, mn = s.top().val;
  while (!s.empty()) {
    const ValFrom v = s.top();
    removeVertex(v.from, g, s);
    if (s.empty() || s.top().val < mn) {
      s.insert(v.from, 0);
      break;
    }
    --cnt;
    mn = s.top().val;
  }
  writeUInt("%u ", cnt);
  for (auto i : s)
    writeUInt("%u ", i.from);
  writeUInt("\n");
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  cin.read(inbuf, sizeof(inbuf));
  for (unsigned int i = readUInt(); i; --i) {
    const unsigned int n = readUInt();
    g1.init(n);
    g2.init(n);
    for (unsigned int j = readUInt(); j; --j) {
      const unsigned int u = readUInt(), v = readUInt();
      g1.addEdge(u, v);
      g2.addEdge(u, v);
    }
    constructSubgraph(n, g1);
    constructVertexSet(n, g2);
  }
  cout.write(outbuf, outpos - outbuf);
  return 0;
}