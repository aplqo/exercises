#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 500000, maxm = 500000;

struct Room {
  unsigned int cost, capacity;
} rom[maxn + 10];
unsigned int caps[maxn + 10];
struct Offer {
  unsigned int price, cap;
} off[maxm + 10];

namespace Disjoint {
unsigned int fa[maxn + 10], siz[maxn + 10], val[maxn + 10];

static void init(const unsigned int n)
{
  iota(fa, fa + 1 + n, 0);
  copy(fa, fa + 1 + n, val);
  fill(siz, siz + 1 + n, 1);
}
unsigned int find(const unsigned int x)
{
  return fa[x] == x ? x : fa[x] = find(fa[x]);
}
inline void mergeTo(unsigned int rt, unsigned int x)
{
  rt = find(rt);
  x = find(x);
  if (siz[rt] < siz[x]) {
    swap(val[rt], val[x]);
    swap(rt, x);
  }
  fa[x] = rt;
  siz[rt] += siz[x];
}
}  // namespace Disjoint
static void init(const unsigned int n, const unsigned int m)
{
  sort(rom, rom + n, [](const Room& a, const Room& b) {
    return a.capacity != b.capacity ? a.capacity < b.capacity : a.cost < b.cost;
  });
  for (unsigned int i = 0; i < n; ++i)
    caps[i] = rom[i].capacity;
  sort(off, off + m, [](const Offer& a, const Offer& b) {
    return a.price != b.price ? a.price > b.price : a.cap < b.cap;
  });
  Disjoint::init(n);
}
unsigned long long greedy(const unsigned int n, const unsigned int m,
                          const unsigned int o)
{
  static unsigned long long prof[maxm + 10];
  init(n, m);
  unsigned long long* pcur = prof;
  for (const Offer* i = off; i < off + m; ++i) {
    const unsigned int mv = Disjoint::val[Disjoint::find(
        lower_bound(caps, caps + n, i->cap) - caps)];
    if (mv == n || rom[mv].cost >= i->price) continue;
    *(pcur++) = i->price - rom[mv].cost;
    Disjoint::mergeTo(mv + 1, mv);
  }
  partial_sort(prof, min(prof + o, pcur), pcur, greater<unsigned long long>());
  return accumulate(prof, min(prof + o, pcur), 0ull);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n, m, o;
  cin >> n >> m >> o;
  for (Room* i = rom; i < rom + n; ++i)
    cin >> i->cost >> i->capacity;
  for (Offer* i = off; i < off + m; ++i)
    cin >> i->price >> i->cap;
  cout << greedy(n, m, o) << "\n";
  return 0;
}