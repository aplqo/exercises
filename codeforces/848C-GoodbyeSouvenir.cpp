#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <set>
const int maxN = 100000, maxM = 100000;
const int maxOp = maxN + maxM * 6;

long long answer[maxM + 10], *qcur = answer;
struct Operate {
  enum class Type { Add, Query } type;
  int x;
  struct {
    struct {
      int y, val;
    } point;
    struct {
      int l;
      long long *ptr;
    } query;
  };
} op[maxOp + 10], *cur = op;

namespace FTree {
#define lowbit(x) ((x) & -(x))
long long val[maxN + 10];
int n;
bool dirty[maxN + 10];
int stack[maxN + 10], *top = stack;

void add(int pos, const int delt)
{
  pos = n + 1 - pos;
  for (; pos <= n; pos += lowbit(pos)) {
    if (!dirty[pos]) {
      dirty[pos] = true;
      *(top++) = pos;
    }
    val[pos] += delt;
  }
}
long long suffixSum(int pos)
{
  pos = n + 1 - pos;
  long long ret = 0;
  for (; pos; pos -= lowbit(pos))
    ret += val[pos];
  return ret;
}
void clear()
{
  for (const int *i = stack; i < top; ++i) {
    dirty[*i] = false;
    val[*i] = 0;
  }
  top = stack;
}
#undef lowbit
}  // namespace FTree
int a[maxN + 10];
std::set<int> position[maxN + 10];

void cdq(Operate *const l, Operate *const r)
{
  static Operate tmp[maxOp + 10];
  if (r - l <= 1) return;
  Operate *const mid = l + (r - l) / 2;
  cdq(l, mid);
  cdq(mid, r);
  std::memcpy(tmp, l, sizeof(*l) * (r - l));

  Operate *const endL = tmp + (r - l) / 2, *const endR = tmp + (r - l);
  Operate *ptrL = tmp, *ptrR = endL, *out = l;
  while (out < r) {
    if (ptrR == endR)
      *(out++) = *(ptrL++);
    else if (ptrL != endL && ptrL->x <= ptrR->x) {
      if (ptrL->type == Operate::Type::Add)
        FTree::add(ptrL->point.y, ptrL->point.val);
      *(out++) = *(ptrL++);
    }
    else {
      if (ptrR->type == Operate::Type::Query)
        *(ptrR->query.ptr) += FTree::suffixSum(ptrR->query.l);
      *(out++) = *(ptrR++);
    }
  }
  FTree::clear();
}

inline void addPoint(const int x, const int y, const int v)
{
  *(cur++) = Operate{Operate::Type::Add, x, {{y, v}, {}}};
}
template <bool isAdd>
inline void changeElement(const int x, const int y)
{
  if constexpr (isAdd)
    addPoint(x, y, x - y);
  else
    addPoint(x, y, y - x);
}
inline void addElement(const int x, const int y) { addPoint(x, y, x - y); }
inline void removeElement(const int x, const int y) { addPoint(x, y, y - x); }

#include <cassert>

template <bool isLast>
void add(const int x, const int shape)
{
  auto &set = position[shape];
  const auto it = set.insert(x).first;
  const int last = *std::prev(it);
  if (last) addElement(x, last);
  if constexpr (!isLast)
    if (auto nxt = std::next(it); nxt != set.end()) {
      const int next = *nxt;
      addElement(next, x);
      if (last) removeElement(next, last);
    }
}
void remove(const int pos, const int shape)
{
  auto &set = position[shape];
  auto it = set.find(pos);

  assert(it != set.end());

  const auto last = *std::prev(it);
  if (last) removeElement(pos, last);
  it = set.erase(it);
  if (it != set.end()) {
    const int next = *it;
    removeElement(next, pos);
    if (last) addElement(next, last);
  }
}
void change(const int pos, const int shape)
{
  remove(pos, a[pos]);
  a[pos] = shape;
  add<false>(pos, shape);
}
void query(const int l, const int r)
{
  *(cur++) = Operate{Operate::Type::Query, r, {{}, {l, qcur++}}};
}
static void init(const int n)
{
  FTree::n = n;
  for (int i = 1; i <= n; ++i)
    position[i].insert(0);
  for (int i = 1; i <= n; ++i)
    add<true>(i, a[i]);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, m;
  std::cin >> n >> m;
  std::copy_n(std::istream_iterator<int>(std::cin), n, a + 1);
  init(n);
  for (int i = 0; i < m; ++i) {
    int type;
    std::cin >> type;
    if (type == 1) {
      int pos, shape;
      std::cin >> pos >> shape;
      change(pos, shape);
    }
    else {
      int l, r;
      std::cin >> l >> r;
      query(l, r);
    }
  }
  cdq(op, cur);
  std::copy(answer, qcur, std::ostream_iterator<long long>(std::cout, "\n"));
  return 0;
}