#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
const int maxN = 3e5, maxQ = 3e5;
const unsigned int mod = 1e9 + 7, inv2 = 500000004;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned int val = l.val + r.val;
    return Number{val >= mod ? val - mod : val};
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{static_cast<unsigned int>(
        (static_cast<unsigned long long>(l.val) * r.val) % mod)};
  }

  unsigned int val;
};

Number pow2[maxN + 10], powInv2[maxN + 10];
unsigned int original[maxN + maxQ + 10];
class SegmentTree {
 public:
  void build(const int l, const int r, SegmentTree*& cur)
  {
    lv = l;
    mv = (l + r) >> 1;
    rv = r;
    count = mul2.val = mulInv2.val = answer.val = 0;
    if (r - l == 1) return;
    lc = cur++;
    lc->build(l, mv, cur);
    rc = cur++;
    rc->build(mv, r, cur);
  }
  template <bool newVal>
  void assign(const int pos)
  {
    if (rv - lv == 1) {
      if constexpr (newVal) {
        count = 1;
        mul2 = Number{original[lv]};
        mulInv2 = Number{original[lv]} * Number{inv2};
      }
      else
        count = mul2.val = mulInv2.val = answer.val = 0;
      return;
    }
    (pos < mv ? lc : rc)->assign<newVal>(pos);
    pushup();
  }

  Number answer;

 private:
  inline void pushup()
  {
    const int countL = lc->count;
    answer = lc->answer + rc->answer + lc->mul2 * rc->mulInv2 * powInv2[countL];
    mul2 = lc->mul2 + rc->mul2 * pow2[countL];
    mulInv2 = lc->mulInv2 + rc->mulInv2 * powInv2[countL];
    count = lc->count + rc->count;
  }

  int lv, mv, rv;
  SegmentTree *lc, *rc;
  int count;
  Number mul2, mulInv2;
} nodes[maxN * 4 + 10], *root = nodes;

unsigned int p[maxN + 10];
struct Modify {
  int pos;
  unsigned int val;
  inline void exec() const;
} modify[maxQ + 10];

static void initPower(const int n)
{
  pow2[0].val = powInv2[0].val = 1;
  for (int i = 1; i <= n; ++i) {
    pow2[i] = pow2[i - 1] * Number{2};
    powInv2[i] = powInv2[i - 1] * Number{inv2};
  }
}
void discretize(const int n, const int q)
{
  static unsigned int cnt[maxN + maxQ + 10];
  std::memcpy(original, p, sizeof(p[0]) * n);
  unsigned int* ptr = original + n;
  for (const Modify* i = modify; i < modify + q; ++i)
    *(ptr++) = i->val;
  std::sort(original, ptr);
  const auto getVal = [ptr](const unsigned int val) {
    const unsigned int base = std::lower_bound(original, ptr, val) - original;
    return base + (cnt[base]++);
  };
  for (unsigned int* i = p; i < p + n; ++i)
    *i = getVal(*i);
  for (Modify* i = modify; i < modify + q; ++i)
    i->val = getVal(i->val);
}
inline void Modify::exec() const
{
  root->assign<false>(p[pos]);
  p[pos] = val;
  root->assign<true>(val);
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  int n, q;
  std::cin >> n;
  std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, p);
  std::cin >> q;
  initPower(n);
  for (Modify* i = modify; i < modify + q; ++i) {
    std::cin >> i->pos >> i->val;
    --(i->pos);
  }
  discretize(n, q);
  {
    SegmentTree* cur = nodes + 1;
    root->build(0, n + q, cur);
  }
  std::for_each(p, p + n, [](unsigned int v) { root->assign<true>(v); });
  std::cout << root->answer.val << "\n";
  for (const Modify* i = modify; i < modify + q; ++i) {
    i->exec();
    std::cout << root->answer.val << "\n";
  }
  return 0;
}