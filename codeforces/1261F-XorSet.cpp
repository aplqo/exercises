#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxN = 100, maxLog = 60;
const int maxNode = maxN * 4 * maxLog * 4,
          maxRange = maxN * 2 * maxN * maxLog * 4;
const unsigned int mod = 998244353, inv2 = 499122177;

struct Range {
  unsigned long long l, r;

  inline friend std::istream &operator>>(std::istream &is, Range &r)
  {
    is >> r.l >> r.r;
    ++r.r;
    return is;
  }
};
template <size_t maxSize>
class Vector {
 public:
  inline const Range *begin() const { return data; }
  inline const Range *end() const { return endPtr; }
  inline void push_back(Range vl) { *(endPtr++) = vl; }
  inline void init() { endPtr = data; }

  inline friend std::istream &operator>>(std::istream &is, Vector &v)
  {
    size_t n;
    is >> n;
    v.endPtr = v.data + n;
    std::copy_n(std::istream_iterator<Range>(is), n, v.data);
    return is;
  }

  Range data[maxSize], *endPtr;
};
typedef Vector<maxN> InputList;
typedef Vector<maxRange> ResultList;

struct Layer {
  Vector<maxN * 4> endpointA, visitB;

  void enumerate(const int dep, ResultList &dest) const;
  inline void init()
  {
    endpointA.init();
    visitB.init();
  }
} ranges[maxLog + 1];

enum class Mark { Endpoint, Visit };
class SegmentTree {
 public:
  template <Mark mrk>
  void mark(const long long l, const long long r);
  void collect(const int dep = 0) const;
  void init(const long long l, const long long r);

  static SegmentTree nodes[maxNode + 10], *cur;

 private:
  static inline SegmentTree *create(const long long l, const long long r);
  static inline SegmentTree *use(SegmentTree *&ptr, const long long l,
                                 const long long r);

  long long lv, mv, rv;
  SegmentTree *lc, *rc;
  bool visitB, endpointA;
} root;
SegmentTree SegmentTree::nodes[maxNode + 10],
    *SegmentTree::cur = SegmentTree::nodes;

template <Mark mrk>
void SegmentTree::mark(const long long l, const long long r)
{
  if constexpr (mrk == Mark::Endpoint) endpointA = true;
  if (l == lv && r == rv) {
    if constexpr (mrk == Mark::Visit) visitB = true;
    return;
  }
  if (r <= mv)
    use(lc, lv, mv)->mark<mrk>(l, r);
  else if (l >= mv)
    use(rc, mv, rv)->mark<mrk>(l, r);
  else {
    use(lc, lv, mv)->mark<mrk>(l, mv);
    use(rc, mv, rv)->mark<mrk>(mv, r);
  }
}
void SegmentTree::collect(const int dep) const
{
  const Range cur{static_cast<unsigned long long>(lv),
                  static_cast<unsigned long long>(rv)};
  if (visitB) ranges[dep].visitB.push_back(cur);
  if (endpointA) ranges[dep].endpointA.push_back(cur);
  if (lc) lc->collect(dep + 1);
  if (rc) rc->collect(dep + 1);
}
inline void SegmentTree::init(const long long l, const long long r)
{
  lv = l;
  mv = (l + r) >> 1;
  rv = r;
  lc = rc = nullptr;
  visitB = endpointA = false;
}
inline SegmentTree *SegmentTree::use(SegmentTree *&ptr, const long long l,
                                     const long long r)
{
  return ptr ? ptr : ptr = create(l, r);
}
inline SegmentTree *SegmentTree::create(const long long l, const long long r)
{
  static SegmentTree nodes[maxNode + 10], *cur = nodes;
  cur->init(l, r);
  return cur++;
}

void Layer::enumerate(const int dep, ResultList &dest) const
{
  const unsigned long long lowMsk = (1ll << (maxLog - dep)) - 1,
                           highMsk = ~lowMsk;
  for (const Range &i : endpointA)
    for (const Range &j : visitB) {
      const unsigned long long prefix = (i.l ^ j.l) & highMsk;
      dest.push_back(Range{prefix, prefix + lowMsk + 1});
    }
}
unsigned int sumRange(ResultList &dest)
{
  static const auto sumInteger = [](unsigned long long l,
                                    unsigned long long r) -> unsigned int {
    return (((l + r) % mod * ((r - l + 1) % mod)) % mod * inv2) % mod;
  };
  unsigned long long ret = 0;
  std::sort(dest.data, dest.endPtr, [](const Range &l, const Range &r) {
    return l.l != r.l ? l.l < r.l : l.r < r.r;
  });
  unsigned long long mxl = 0;
  for (const Range &i : dest) {
    mxl = std::max(mxl, i.l);
    if (i.r > mxl) {
      ret += sumInteger(mxl, i.r - 1);
      mxl = i.r;
    }
  }
  return ret % mod;
}
void init()
{
  SegmentTree::cur = SegmentTree::nodes;
  for (auto &i : ranges)
    i.init();
}
void sumXor(const InputList &a, const InputList &b, ResultList &dest)
{
  init();
  root.init(0, 1ll << maxLog);
  std::for_each<const Range *>(a.data, a.endPtr, [](const Range &i) {
    root.mark<Mark::Endpoint>(i.l, i.r);
  });
  std::for_each<const Range *>(b.data, b.endPtr, [](const Range &i) {
    root.mark<Mark::Visit>(i.l, i.r);
  });
  root.collect();
  for (int i = 0; i <= maxLog; ++i)
    ranges[i].enumerate(i, dest);
}

int main()
{
  std::ios::sync_with_stdio(false);
  static Vector<maxN> a, b;
  std::cin >> a >> b;
  static ResultList result;
  result.init();
  sumXor(a, b, result);
  sumXor(b, a, result);
  std::cout << sumRange(result) << "\n";
  return 0;
}