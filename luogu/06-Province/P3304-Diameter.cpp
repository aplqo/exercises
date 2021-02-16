#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <utility>
using namespace std;
const unsigned int maxn = 200000;

struct Edge {
  unsigned int to, val;
  const Edge* pre;
} ed[maxn * 2 + 10];
const Edge* head[maxn + 10];
bool onDiameter[maxn + 10];
unsigned int nxt[maxn + 10], index[maxn + 10];
unsigned long long disL[maxn + 10], disR[maxn + 10], disSub[maxn + 10];

inline void addEdge(const unsigned int from, const unsigned int to,
                    const unsigned int val)
{
  static Edge* cur = ed;
  new (cur) Edge{to, val, head[from]};
  head[from] = cur++;
  new (cur) Edge{from, val, head[to]};
  head[to] = cur++;
}
typedef pair<unsigned long long, unsigned int> Path;
inline Path operator+(const Path p, const unsigned long long v)
{
  return Path{p.first + v, p.second};
}

Path dfsPath(const unsigned int x, const unsigned int f)
{
  Path ret(0, x);
  for (const Edge* i = head[x]; i; i = i->pre)
    if (i->to != f && !onDiameter[i->to])
      ret = max(ret, dfsPath(i->to, x) + i->val);
  return ret;
}
Path dfsPathMark(const unsigned int x, const unsigned int f)
{
  Path ret(0, x);
  for (const Edge* i = head[x]; i; i = i->pre) {
    const unsigned int to = i->to;
    if (to == f) continue;
    const Path cp = dfsPathMark(to, x) + i->val;
    if (cp > ret) {
      ret = cp;
      nxt[x] = to;
      disL[x] = i->val;
    }
  }
  return ret;
}
void markDiameter(const unsigned int endp, const unsigned long long len)
{
  unsigned int idx = 0;
  unsigned long long dis = 0;
  for (unsigned int i = endp; i; i = nxt[i]) {
    const unsigned long long od = dis;
    dis += disL[i];
    disL[i] = od;
    disR[i] = len - disL[i];
    index[i] = idx;
    onDiameter[i] = true;
    ++idx;
  }
}
pair<unsigned int, unsigned int> findDiameter()
{
  const unsigned int pos = dfsPath(1, 0).second;
  const auto [dis, endp] = dfsPathMark(pos, 0);
  markDiameter(pos, dis);
  return make_pair(pos, endp);
}
bool getDisSub(const unsigned int endp, const unsigned long long dia)
{
  for (unsigned int i = endp; i; i = nxt[i]) {
    const auto [val, pos] = dfsPath(i, 0);
    if (dfsPath(pos, 0).second == dia) return false;
    disSub[i] = val;
  }
  return true;
}
unsigned int coutEdge(const unsigned int endl, const unsigned long long endr)
{
  unsigned int pl = 0, pr = index[endr];
  for (unsigned int i = endl; i; i = nxt[i]) {
    if (disSub[i] == disL[i]) pl = max(pl, index[i]);
    if (disSub[i] == disR[i]) pr = min(pr, index[i]);
  }
  return pl <= pr ? pr - pl : 0;
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int u, v, val;
    cin >> u >> v >> val;
    addEdge(u, v, val);
  }
  const auto [vl, vr] = findDiameter();
  cout << disR[vl] << "\n";
  cout << (getDisSub(vl, disR[vl]) ? coutEdge(vl, vr) : 0) << "\n";
  return 0;
}