#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <limits>
#include <queue>
using namespace std;
const unsigned int maxl = 50, maxk = 100, maxn = maxk, maxg = maxn + 2,
                   maxm = 50;
constexpr unsigned int maxv = maxg * 2, maxe = maxk * 2, avert = maxm * 2;
typedef unsigned long long Length;
constexpr Length inf = numeric_limits<Length>::max() / 3;
template <class T>
using StateMatrix = T[maxv + 10][avert + 10][avert + 10];

class MutationGraph {
 public:
  struct Mutation {
    unsigned int to, another;
    const Mutation* pre;
  } mut[maxe + 10];
  const Mutation* head[maxv + 10];

  inline void add(const unsigned int from, const unsigned int to,
                  const unsigned int another)
  {
    new (cur) Mutation{to, another, head[from]};
    head[from] = cur++;
  }

 private:
  Mutation* cur = mut;
} single, asFirst, asSecond;
StateMatrix<Length> dis;
unsigned int gcur;

namespace AhoCorasick {
unsigned int ch[avert + 10][2], fail[avert + 10], id[avert + 10];
bool isEnd[avert + 10];
unsigned int vcur;

void readString()
{
  unsigned int len;
  cin >> len;
  unsigned int ptr = 0;
  for (unsigned int i = 0; i < len; ++i) {
    unsigned int cb;
    cin >> cb;
    if (!ch[ptr][cb]) ch[ptr][cb] = ++vcur;
    ptr = ch[ptr][cb];
  }
  isEnd[ptr] = true;
}
unsigned int reLabel()
{
  unsigned int icur = 0;
  for (unsigned int i = 0; i <= vcur; ++i)
    if (!isEnd[i]) id[i] = icur++;
  return icur;
}
unsigned int build()
{
  queue<unsigned int> q;
  for (unsigned int i : ch[0])
    if (i) q.push(i);
  while (!q.empty()) {
    const unsigned int cur = q.front();
    q.pop();
    for (unsigned int i = 0; i < 2; ++i)
      if (!ch[cur][i])
        ch[cur][i] = ch[fail[cur]][i];
      else {
        const unsigned int cv = ch[cur][i];
        fail[cv] = ch[fail[cur]][i];
        isEnd[cv] = isEnd[cv] || isEnd[fail[cv]];
        q.push(cv);
      }
  }
  return reLabel();
}
}  // namespace AhoCorasick
void addMutation(const unsigned int a)
{
  static unsigned int buf[maxk + 10];
  unsigned int k;
  cin >> k;
  copy_n(istream_iterator<unsigned int>(cin), k, buf);
  if (k == 1) {
    single.add(buf[0], a, 0);
    return;
  }
  unsigned int lst = buf[k - 1];
  for (const unsigned int* i = buf + k - 2; i > buf; --i) {
    const unsigned int virt = gcur++;
    asFirst.add(*i, virt, lst);
    asSecond.add(lst, virt, *i);
    lst = virt;
  }
  asFirst.add(buf[0], a, lst);
  asSecond.add(lst, a, buf[0]);
}
struct StateNode {
  unsigned int id, start, end;
  Length dis;

  template <class T>
  inline T& operator[](StateMatrix<T> mat) const
  {
    return mat[id][start][end];
  }
};
struct StateLess {
  inline bool operator()(const StateNode& a, const StateNode& b) const
  {
    return a.dis > b.dis;
  }
};
typedef priority_queue<StateNode, vector<StateNode>, StateLess> Heap;
static void dpInit(Heap& q)
{
  using namespace AhoCorasick;
  for (unsigned int i = 0; i <= vcur; ++i) {
    if (isEnd[i]) continue;
    for (unsigned int j = 0; j < 2; ++j) {
      const unsigned int nvert = ch[i][j];
      if (!isEnd[nvert]) {
        dis[j][id[i]][id[nvert]] = 1;
        q.push(StateNode{j, id[i], id[nvert], 1});
      }
    }
  }
}
void dijkstra(const unsigned int acId)
{
  static StateMatrix<bool> vis;
  Heap q;
  fill(reinterpret_cast<Length*>(dis),
       reinterpret_cast<Length*>(dis) + sizeof(dis) / sizeof(Length), inf);
  dpInit(q);
  auto update = [&q](const StateNode&& sn) {
    if (sn[dis] > sn.dis) {
      sn[dis] = sn.dis;
      q.push(sn);
    }
  };
  while (!q.empty()) {
    const StateNode cur = q.top();
    q.pop();
    if (cur[vis]) continue;
    cur[vis] = true;
    for (auto i = single.head[cur.id]; i; i = i->pre)
      update(StateNode{i->to, cur.start, cur.end, cur.dis});
    for (auto i = asFirst.head[cur.id]; i; i = i->pre)
      for (unsigned int j = 0; j < acId; ++j)
        update(StateNode{i->to, cur.start, j,
                         cur.dis + dis[i->another][cur.end][j]});
    for (auto i = asSecond.head[cur.id]; i; i = i->pre)
      for (unsigned int j = 0; j < acId; ++j)
        update(StateNode{i->to, j, cur.end,
                         dis[i->another][j][cur.start] + cur.dis});
  }
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int g, n, m;
  cin >> g >> n >> m;
  gcur = g;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int a;
    cin >> a;
    addMutation(a);
  }
  for (unsigned int i = 0; i < m; ++i)
    AhoCorasick::readString();
  const unsigned int acId = AhoCorasick::build();
  dijkstra(acId);
  for (unsigned int i = 2; i < g; ++i) {
    Length ans = inf;
    for (unsigned int s = 0; s < acId; ++s)
      ans = min(ans, *min_element(dis[i][s], dis[i][s] + acId));
    if (ans < inf)
      cout << "NO " << ans << "\n";
    else
      cout << "YES\n";
  }
  return 0;
}