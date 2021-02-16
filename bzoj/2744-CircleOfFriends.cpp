#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <iterator>
#include <queue>
#include <vector>
using namespace std;
const unsigned int maxa = 200, maxb = 3000;
constexpr int inf = INT_MAX / 2;

struct DirEdge  // directed edge
{
  unsigned int to;
  const DirEdge* pre;
} ded[maxa * maxb], *dcur = ded;
const DirEdge* dhead[maxa + 10];
unsigned int enable[maxb + 10];
vector<unsigned int> vertexa[2], vertexb[2];

inline void addDirEdge(const unsigned int from, const unsigned int to)
{
  new (dcur) DirEdge{to, dhead[from]};
  dhead[from] = dcur++;
}
namespace Flow {
struct Edge {
  unsigned int from, to;
  int cap;
  mutable int flw;
  const Edge *pre, *rev;
} ed[maxb * maxb * 2], *ecur = ed;
const Edge *head[maxb + 10], *par[maxb + 10];
unsigned int dep[maxb + 10];

inline Edge* addDirEdge(const unsigned int from, const unsigned int to,
                        const int cap)
{
  new (ecur) Edge{from, to, cap, 0, head[from]};
  head[from] = ecur;
  return ecur++;
}
void addEdge(const unsigned int from, const unsigned int to, const int cap)
{
  Edge *a = addDirEdge(from, to, cap), *b = addDirEdge(to, from, 0);
  a->rev = b;
  b->rev = a;
}

template <unsigned int tok>
void bfs(const unsigned int t)
{
  static unsigned int vis[maxb + 10], sta = 0;
  queue<unsigned int> q;
  ++sta;
  q.push(t);
  dep[t] = 0;
  vis[t] = sta;
  while (!q.empty()) {
    const unsigned int x = q.front();
    q.pop();
    for (const Edge* i = head[x]; i; i = i->pre)
      if (enable[i->to] == tok && vis[i->to] != sta &&
          i->rev->cap > i->rev->flw) {
        dep[i->to] = dep[x] + 1;
        vis[i->to] = sta;
        q.push(i->to);
      }
  }
}
int augment(const unsigned int s, const unsigned int t)
{
  int ret = inf;
  unsigned int x = t;
  while (x != s) {
    ret = min(ret, par[x]->cap - par[x]->flw);
    x = par[x]->from;
  }
  x = t;
  while (x != s) {
    par[x]->flw += ret;
    par[x]->rev->flw -= ret;
    x = par[x]->from;
  }
  return ret;
}
void clear()
{
  for (Edge* i = ed; i < ecur; ++i)
    i->flw = 0;
}
template <unsigned int tok>
unsigned int isap(const unsigned int nb, const unsigned int s,
                  const unsigned int t)
{
  static unsigned int cnt[maxb + 10];
  static const Edge* cur[maxb + 10];
  unsigned int n = 0;
  enable[s] = enable[t] = tok;
  clear();
  bfs<tok>(t);
  memset(cnt, 0, sizeof(cnt));
  copy(head, head + nb, cur);
  for (unsigned int i = 0; i < nb; ++i)
    if (enable[i] == tok) {
      ++cnt[dep[i]];
      ++n;
    }
  unsigned int ret = 0, x = s;
  while (dep[x] < n) {
    if (x == t) {
      ret += augment(s, t);
      x = s;
    }
    bool adv = false;
    for (const Edge*& i = cur[x]; i; i = i->pre)
      if (enable[i->to] == tok && dep[i->to] + 1 == dep[x] && i->cap > i->flw) {
        adv = true;
        par[i->to] = i;
        x = i->to;
        break;
      }
    if (!adv) {
      unsigned int miv = n;
      for (const Edge* i = head[x]; i; i = i->pre)
        if (enable[i->to] == tok && i->cap > i->flw)
          miv = min(miv, dep[i->to] + 1);
      --cnt[dep[x]];
      if (!cnt[dep[x]]) break;
      dep[x] = miv;
      ++cnt[dep[x]];
      cur[x] = head[x];
      if (x != s) x = par[x]->from;
    }
  }
  return ret;
}
}  // namespace Flow
unsigned int clique(const unsigned int na, const unsigned int nb)
{
  static const auto mark = [](const unsigned int x) {
    for (auto i = dhead[x]; i; i = i->pre)
      ++enable[i->to];
  };
  if (vertexb[0].size() > vertexb[1].size()) vertexb[0].swap(vertexb[1]);
  unsigned int ret = nb - Flow::isap<0>(nb + 2, 0, nb + 1);
  for (unsigned int a : vertexa[0])
    for (unsigned int b : vertexa[1]) {
      mark(a);
      mark(b);
      const unsigned int c = count(enable + 1, enable + 1 + nb, 2);
      if (c + 2 > ret) ret = max(ret, c + 2 - Flow::isap<2>(nb + 2, 0, nb + 1));
      memset(enable, 0, sizeof(enable));
    }
  for (unsigned int i = 1; i <= na; ++i) {
    mark(i);
    const unsigned int c = count(enable, enable + 1 + nb, 1);
    if (c + 1 > ret) ret = max(ret, c - Flow::isap<1>(nb + 2, 0, nb + 1) + 1);
    memset(enable, 0, sizeof(enable));
  }
  return ret;
}
inline unsigned int popcnt(unsigned int x)
{
  unsigned int ret = 0;
  for (; x; x -= x & (-x))
    ++ret;
  return ret;
}
unsigned int solve()
{
  static unsigned int vb[maxb + 10];
  unsigned int na, nb, m;
  cin >> na >> nb >> m;
  for (unsigned int i = 1; i <= na; ++i) {
    unsigned int v;
    cin >> v;
    vertexa[v & 0x01].emplace_back(i);
  }
  copy_n(istream_iterator<unsigned int>(cin), nb, vb + 1);
  for (unsigned int i = 0; i < m; ++i) {
    unsigned int x, y;
    cin >> x >> y;
    addDirEdge(x, y);
  }
  for (unsigned int i = 1; i <= nb; ++i)
    vertexb[vb[i] & 0x01].emplace_back(i);
  for (unsigned int v1 : vertexb[0]) {
    Flow::addEdge(0, v1, 1);
    for (unsigned int v2 : vertexb[1])
      if (!(popcnt(vb[v1] | vb[v2]) & 0x01)) Flow::addEdge(v1, v2, inf);
  }
  for (unsigned int v2 : vertexb[1])
    Flow::addEdge(v2, nb + 1, 1);
  return clique(na, nb);
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  cin >> t;
  for (auto& i : vertexb)
    i.reserve(maxb);
  for (auto& i : vertexa)
    i.reserve(maxa);
  for (unsigned int i = 0; i < t; ++i) {
    cout << solve() << "\n";
    memset(dhead, 0, sizeof(dhead));
    dcur = ded;
    memset(Flow::head, 0, sizeof(Flow::head));
    Flow::ecur = Flow::ed;
    for (auto& i : vertexa)
      i.clear();
    for (auto& i : vertexb)
      i.clear();
  }
  return 0;
}