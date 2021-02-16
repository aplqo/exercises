#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cstring>
#include <iostream>
#include <iterator>
#include <vector>
using namespace std;
const unsigned int maxn = 3e5, maxl = 16, maxm = 3e5;
constexpr long long inf = 1e9 * maxn;

struct Edge {
  unsigned int to, val;
  Edge* pre;
};
Edge* head[maxn + 10];
struct Cand {
  unsigned long long val;
  unsigned int sub;
  inline bool operator<(const Cand& r) const { return val < r.val; }
};
template <class T, unsigned int siz>
class Vector {
 public:
  inline const T* cbegin() const { return val; }
  inline const T* cend() const { return ptr; }
  inline T* begin() { return val; }
  inline T* end() { return ptr; }
  inline void push_back(T val) { *(ptr++) = val; }
  inline void clear() { ptr = val; }
  inline unsigned int size() const { return ptr - val; }

 private:
  T val[siz], *ptr = val;
};
unsigned long long dis[maxn + 10][maxl + 10], disRoot[maxn + 10];
unsigned int fa[maxn + 10][maxl + 10], dep[maxn + 10], lg[maxn + 10];
unsigned int armys[maxm + 10];
unsigned int mrk[maxn + 10], sta;
bool vis[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  static Edge ed[maxn * 2 + 10], *cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, val, head[to]};
  head[to] = cur++;
}
static void getLog(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    lg[i] = lg[i >> 1] + 1;
}
void dfsInit(const unsigned int x, const unsigned int f)
{
  dep[x] = dep[f] + 1;
  fa[x][0] = f;
  for (unsigned int i = 1; i <= lg[dep[x]]; ++i) {
    fa[x][i] = fa[fa[x][i - 1]][i - 1];
    dis[x][i] = dis[x][i - 1] + dis[fa[x][i - 1]][i - 1];
  }
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f) {
      dis[i->to][0] = i->val;
      disRoot[i->to] = disRoot[x] + i->val;
      dfsInit(i->to, x);
    }
}
static void init(const unsigned int n, const unsigned int m)
{
  static unsigned int tmp[maxn + 10];
  getLog(n);
  dfsInit(1, 0);
  const auto disLess = [](const unsigned int a, const unsigned int b) {
    return disRoot[a] < disRoot[b];
  };
  sort(armys, armys + m, disLess);
  {
    unsigned int* cur = tmp;
    for (const Edge* i = head[1]; i; i = i->pre)
      *(cur++) = i->to;
    sort(tmp, cur, disLess);
    for (Edge* i = head[1]; i; i = i->pre) {
      i->to = *(--cur);
      i->val = disRoot[i->to];
    }
  }
}

unsigned int kthFa(unsigned int x, unsigned int k)
{
  for (unsigned int i = 0, msk = 1; k; ++i, msk <<= 1)
    if (k & msk) {
      x = fa[x][i];
      k ^= msk;
    }
  return x;
}
unsigned int findHighist(unsigned int x, unsigned long long tim)
{
  for (unsigned int i = lg[dep[x] - 1] + 1; i && tim && dep[x] > 2; --i) {
    const unsigned int m = i - 1;
    if (fa[x][m] > 1 && dis[x][m] <= tim) {
      tim -= dis[x][m];
      x = fa[x][m];
    }
  }
  return x;
}
template <bool root>
bool dfsTree(const unsigned int x, const unsigned int f)
{
  vis[x] = true;
  if (mrk[x] == sta) return true;
  unsigned int cnt = 0;
  for (const Edge* i = head[x]; i && vis[x]; i = i->pre)
    if (i->to != f) {
      if constexpr (root)
        dfsTree<false>(i->to, x);
      else
        vis[x] = vis[x] && dfsTree<false>(i->to, x);
      ++cnt;
    }
  return vis[x] = vis[x] && cnt;
}
const Vector<Cand, maxm>* removeInvalid(const Vector<Cand, maxm>* rest)
{
  static Vector<Cand, maxm> ret;
  ret.clear();
  dfsTree<true>(1, 0);
  for (auto it = rest->cbegin(); it != rest->cend(); ++it) {
    if (vis[it->sub] || it->val >= dis[it->sub][0])
      ret.push_back(*it);
    else if (it->val < dis[it->sub][0])
      vis[it->sub] = true;
  }
  return &ret;
}
const Vector<Cand, maxm>* addArmy(const unsigned int m,
                                  const unsigned long long tim)
{
  static Vector<Cand, maxm> rest;
  rest.clear();
  ++sta;
  for (const unsigned int* i = armys; i < armys + m; ++i) {
    if (disRoot[*i] < tim)
      rest.push_back(Cand{tim - disRoot[*i], kthFa(*i, dep[*i] - 2)});
    else
      mrk[findHighist(*i, tim)] = sta;
  }
  return removeInvalid(&rest);
}
bool checkReach(const Vector<Cand, maxm>* army)
{
  static Vector<Cand, maxm> city;
  city.clear();
  for (const Edge* i = head[1]; i; i = i->pre)
    if (!vis[i->to]) city.push_back(Cand{i->val, i->to});
  if (army->size() < city.size()) return false;
  auto cit = city.cbegin();
  for (auto ait = army->cbegin(); cit != city.cend() && ait != army->cend();
       ++ait)
    if (cit->val <= ait->val) ++cit;
  return cit == city.cend();
}
inline bool check(const unsigned int m, const unsigned long long time)
{
  memset(vis, 0, sizeof(vis));
  return checkReach(addArmy(m, time));
}
unsigned long long binary(const unsigned int m)
{
  long long l = 0, r = inf;
  while (l <= r) {
    const long long mid = (l + r) / 2;
    if (check(m, mid))
      r = mid - 1;
    else
      l = mid + 1;
  }
  return l;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b, c;
    cin >> a >> b >> c;
    addEdge(a, b, c);
  }
  cin >> m;
  copy_n(istream_iterator<unsigned int>(cin), m, armys);
  init(n, m);
  const unsigned long long v = binary(m);
  if (v >= inf)
    cout << "-1\n";
  else
    cout << v << "\n";
  return 0;
}