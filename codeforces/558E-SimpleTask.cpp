#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <array>
#include <cstring>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
const unsigned int maxn = 1e5, maxm = 1e5, maxc = 26;
const unsigned int maxNode = maxn * 4;
const size_t listSize = sizeof(unsigned int) * maxc;

enum Order { Decrease = 0, Increase = 1 };
unsigned int mem[maxNode + 10][maxc], *freeList[maxNode + 10], **top;
struct SortedInterval {
  unsigned int l;
  mutable unsigned int r;
  mutable Order order;
  mutable unsigned int* count;

  inline bool operator<(const SortedInterval& rhs) const { return l < rhs.l; }
  std::pair<unsigned int*, unsigned int*> split(unsigned int idx) const;
  void print() const;
};
std::set<SortedInterval> interval;
char str[maxn + 10];

std::pair<unsigned int*, unsigned int*> SortedInterval::split(
    unsigned int idx) const
{
  if (order == Order::Decrease) idx = r - l - idx;
  unsigned int* ret = *(--top);
  unsigned int i = 0;
  for (; i < maxc && count[i] <= idx; ++i) {
    ret[i] = count[i];
    idx -= count[i];
    count[i] = 0;
  }
  if (idx) {
    ret[i] = idx;
    count[i] -= idx;
    ++i;
  }
  std::fill(ret + i, ret + maxc, 0);
  return order == Order::Increase ? std::make_pair(ret, count)
                                  : std::make_pair(count, ret);
}
void SortedInterval::print() const
{
  const int step = order == Order::Increase ? 1 : -1;
  for (int i = order == Order::Increase ? 0 : maxc - 1; i < maxc && i >= 0;
       i += step)
    for (unsigned int j = count[i]; j; --j)
      std::cout.put(i + 'a');
}
static void init(const unsigned int n)
{
  for (unsigned int i = 0; i < maxNode; ++i)
    freeList[i] = mem[i];
  top = freeList + maxNode;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int* const lst = *(--top);
    lst[str[i] - 'a'] = 1;
    interval.insert(SortedInterval{i, i + 1, Order::Increase, lst});
  }
}
void merge(unsigned int* to, unsigned int* from)
{
  for (unsigned int i = 0; i < maxc; ++i)
    to[i] += from[i];
  *(top++) = from;
}
void sort(const unsigned int l, const unsigned int r, const Order order)
{
  auto left = std::prev(interval.upper_bound(SortedInterval{l})),
       right = std::prev(interval.upper_bound(SortedInterval{r}));
  unsigned int* count = *(--top);
  memset(count, 0, listSize);
  if (left == right) {
    SortedInterval si = *left;
    if (left->l == l && left->r == r) {
      left->order = order;
      return;
    }
    interval.erase(left);
    if (si.l < l) {
      const auto [p, n] = si.split(l - si.l);
      si.count = n;
      interval.insert(SortedInterval{si.l, l, si.order, p});
      si.l = l;
    }
    if (si.r > r) {
      const auto [p, n] = si.split(r - si.l);
      count = p;
      interval.insert(SortedInterval{r, si.r, si.order, n});
    }
    else
      count = si.count;
  }
  else {
    if (left->l != l) {
      const auto [p, n] = left->split(l - left->l);
      merge(count, n);
      left->count = p;
      left->r = l;
      ++left;
    }
    while (left != right && left->r <= r) {
      merge(count, left->count);
      left = interval.erase(left);
    }
    if (right->l < r) {
      if (right->r > r) {
        const auto [p, n] = right->split(r - right->l);
        merge(count, p);
        interval.insert(SortedInterval{
            r,
            right->r,
            right->order,
            n,
        });
      }
      else
        merge(count, right->count);
      interval.erase(right);
    }
  }
  interval.insert(SortedInterval{l, r, order, count});
}
void print()
{
  for (const auto& i : interval)
    i.print();
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  unsigned int n, m;
  std::cin >> n >> m >> (str + 1);
  init(n);
  for (unsigned int i = m; i; --i) {
    unsigned int op, l, r;
    std::cin >> l >> r >> op;
    sort(l, r + 1, static_cast<Order>(op));
  }
  print();
  return 0;
}