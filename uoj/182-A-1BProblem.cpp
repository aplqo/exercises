#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const unsigned int maxn = 100000, maxm = 60000;
const unsigned int maxlp = 19, maxp = 1u << maxlp;
constexpr unsigned long long mod = 998244353, primitiveRoot = 3;

struct Number {
#define decl(x)                                                   \
  inline friend Number operator x(const Number l, const Number r) \
  {                                                               \
    return Number{((l.val + mod) x r.val) % mod};                 \
  }                                                               \
  inline void operator x##=(const Number r)                       \
  {                                                               \
    val = ((val + mod) x r.val) % mod;                            \
  }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline Number operator-() const { return Number{val ? mod - val : 0}; }
  inline friend std::ostream& operator<<(std::ostream& os, const Number v)
  {
    return os << v.val;
  }
  inline friend std::istream& operator>>(std::istream& is, Number& v)
  {
    return is >> v.val;
  }
  inline bool isZero() const { return !val; };
  inline bool operator==(const unsigned long long r) const { return val == r; }

  unsigned long long val;
};
template <class T>
struct Pointer {
  T* ptr;
  unsigned int size;

  template <class U>
  inline operator Pointer<U>() const
  {
    return Pointer<U>{ptr, size};
  }
  template <unsigned long long v = 0>
  inline void shrink()
  {
    while (size && ptr[size - 1] == v)
      --size;
  }
};
typedef Pointer<Number> ArrayPtr;
typedef Pointer<const Number> ConstArrayPtr;

struct Transform {
  union {
    struct {
      Number a, b, c;  // a + b/(x + c)
    };
    struct {
      Number acc, div;
    };
  };
  bool isFrac;
} trans[maxm + 10];
Number ans[maxm + 10], a[maxn + 10];

Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number v) { return quickPow(v, mod - 2); }
inline void add(const unsigned int id, const Number x)
{
  trans[id] = trans[id - 1];
  if (trans[id].isFrac)
    trans[id].a += x;
  else
    trans[id].acc += trans[id].div * x;
}
inline void inverse(const unsigned int id)
{
  Transform &cur = trans[id], &lst = trans[id - 1];
  if (trans[id - 1].isFrac) {
    if (trans[id - 1].a.isZero()) {
      cur.isFrac = false;
      cur.acc = trans[id - 1].c;
      cur.div = trans[id - 1].b;
    }
    else {
      const Number iva = quickPow(lst.a, mod - 2);
      cur.a = iva;
      cur.b = -lst.b * iva * iva;
      cur.c = lst.c + lst.b * iva;
      cur.isFrac = true;
    }
  }
  else {
    cur.a = Number{0};
    cur.b = lst.div;
    cur.c = lst.acc;
    cur.isFrac = true;
  }
}

namespace Polynomial {
Number unit[2][maxp + 10];
unsigned int rev[maxlp + 1][maxp + 10], ceilLg[maxp + 10];

static void initUnit(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unit[0][i] = quickPow(Number{primitiveRoot}, (mod - 1) >> i);
    unit[1][i] = quickPow(unit[0][i], mod - 2);
  }
}
static void initRev(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i) {
    unsigned int* const crev = rev[i];
    for (unsigned int j = 0; j < i; ++j)
      crev[1u << j] = 1u << (i - 1 - j);
    const unsigned int n = 1u << i;
    for (unsigned int j = 3; j < n; ++j) {
      const unsigned int lb = j & -j;
      crev[j] = crev[j ^ lb] | crev[lb];
    }
  }
}
static void initLog(const unsigned int lgn)
{
  for (unsigned int i = 1; i <= lgn; ++i)
    std::fill(ceilLg + (1u << (i - 1)) + 1, ceilLg + (1u << i) + 1, i);
}
unsigned int Log2(const unsigned int n)
{
  for (unsigned int i = 0;; ++i)
    if (n <= (1u << i)) return i;
  return 0;
}
static void init(const unsigned int n)
{
  const unsigned int lgn = Log2(n);
  initUnit(lgn + 2);
  initRev(lgn + 2);
  initLog(lgn + 2);
}

void reverseCopy(Number dest[], const ConstArrayPtr src, const unsigned int lgn)
{
  static const Number zero{};
  const unsigned int* const crev = rev[lgn];
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < src.size; ++i)
    dest[crev[i]] = src.ptr[i];
  for (unsigned int i = src.size; i < n; ++i)
    dest[crev[i]] = zero;
}
template <bool inv>
void dft(const ConstArrayPtr& a, Number dest[], const unsigned int lgn)
{
  reverseCopy(dest, a, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 1; i <= lgn; ++i) {
    const unsigned int step = 1u << i, half = 1u << (i - 1);
    const Number wm = unit[inv][i];
    for (Number* blk = dest; blk < dest + n; blk += step) {
      Number w{1};
      for (Number *j = blk, *k = blk + half; j < blk + half; ++j, ++k) {
        const Number u = *j, v = *k * w;
        *j = u + v;
        *k = u - v;
        w *= wm;
      }
    }
  }
  if (inv) {
    const Number iv = quickPow(Number{n}, mod - 2);
    for (Number* i = dest; i < dest + n; ++i)
      *i *= iv;
  }
}

template <bool op1 = false, bool op2 = false, bool op3 = true, class Func>
void convolution(const ConstArrayPtr& a, const ConstArrayPtr& b,
                 const ArrayPtr& dest, Func pred)
{
  static Number tmpa[maxp + 10], tmpb[maxp + 10];
  const unsigned int lgn = ceilLg[dest.size], n = 1u << lgn;
  dft<op1>(a, tmpa, lgn);
  dft<op2>(b, tmpb, lgn);
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] = pred(tmpa[i], tmpb[i]);
  dft<op3>(ConstArrayPtr{tmpb, n}, dest.ptr, lgn);
}
template <bool op1 = false, bool op2 = false, bool op3 = true>
inline void ntt(const ConstArrayPtr& a, const ConstArrayPtr& b, ArrayPtr& dest)
{
  dest.size = a.size + b.size - 1;
  convolution<op1, op2, op3>(a, b, dest, std::multiplies<Number>());
}
inline void updateInverse(const ConstArrayPtr& a, const Number old[],
                          Number dest[], const unsigned int n)
{
  convolution(a, ConstArrayPtr{old, n >> 1}, ArrayPtr{dest, n << 1},
              [](Number a, Number b) { return b * (Number{2} - a * b); });
}
void inverse(const ConstArrayPtr a, const ArrayPtr dest)
{
  Number* const pDest = dest.ptr;
  pDest[0] = inverse(a.ptr[0]);
  if (dest.size == 1) return;
  const unsigned int n = 1u << ceilLg[dest.size],
                     lim = std::max(2u, 1u << ceilLg[a.size]);
  for (unsigned int i = 2; i < lim; i <<= 1)
    updateInverse(ConstArrayPtr{a.ptr, i}, pDest, pDest, i);
  for (unsigned int i = lim; i <= n; i <<= 1)
    updateInverse(a, pDest, pDest, i);
}
inline void derivative(const ConstArrayPtr a, ArrayPtr& dest)
{
  for (unsigned int i = 0; i + 1 < a.size; ++i)
    dest.ptr[i] = a.ptr[i + 1] * Number{i + 1};
  dest.size = a.size - 1;
}
Number eval(const ConstArrayPtr pol, const Number x)
{
  Number ret = pol.ptr[pol.size - 1];
  if (pol.size > 1) {
    for (unsigned int i = pol.size - 1; i; --i)
      ret = ret * x + pol.ptr[i - 1];
  }
  return ret;
}

namespace Product {
template <bool inPlace>
inline void multiply(ConstArrayPtr& now, const ConstArrayPtr* left,
                     Number*& cur)
{
  ArrayPtr tp;
  if (inPlace)
    tp.ptr = const_cast<Number*>(now.ptr);
  else
    tp.ptr = cur;
  ntt(*left, *(left + 1), tp);
  if (!inPlace) cur += tp.size;
  now = tp;
}
template <bool inPlace, class Func>
void product(const ConstArrayPtr a, const unsigned int lgn,
             ConstArrayPtr* const prod, Number*& cur, Func ini)
{
  const unsigned int msk = 1u << lgn;
  {
    for (unsigned int i = 0; i < a.size; ++i) {
      ini(cur, a.ptr[i]);
      prod[i | msk] = ConstArrayPtr{cur, 2};
      cur += 2;
    }
    if (a.size < msk) {
      *cur = Number{1};
      const unsigned int cid = a.size | msk;
      prod[cid] = ConstArrayPtr{cur, 1};
      ++cur;
      if (cid + 1 < (msk << 1))
        std::fill(prod + cid + 1, prod + (msk << 1), prod[cid]);
    }
  }
  for (unsigned int cid = msk - 1; cid > 1; --cid) {
    const unsigned int left = cid << 1, right = left | 1;
    if (prod[right].size == 1 && prod[right].ptr[0] == 1)
      prod[cid] = prod[left];
    else
      multiply<false>(prod[cid], prod + left, cur);
  }
  multiply<inPlace>(prod[1], prod + 2, cur);
}

template <class Func>
inline void product(const ConstArrayPtr& a, ArrayPtr& dest, Func ini)
{
  if (a.size == 1) {
    ini(dest.ptr, a.ptr[0]);
    dest.size = 2;
    return;
  }
  static Number mem[maxp * maxlp * 4 + 10];
  static ConstArrayPtr prod[maxp * 4 + 10];
  prod[1] = ConstArrayPtr{dest.ptr, 0};
  Number* cur = mem;
  product<true>(a, ceilLg[a.size], prod, cur, ini);
  dest.size = prod[1].size;
}
}  // namespace Product
namespace Evaluation {
Number mem[maxp * maxlp * 4 * 2 + 10], *cur = mem;
ConstArrayPtr t[maxp * 4 + 10], prod[maxp * 4 + 10];

ConstArrayPtr transMultiply(const Number vx[], const ConstArrayPtr& b,
                            const unsigned int lgn)
{
  static Number tmpb[maxp + 10];
  dft<false>(b, tmpb, lgn);
  const unsigned int n = 1u << lgn;
  for (unsigned int i = 0; i < n; ++i)
    tmpb[i] *= vx[i];
  dft<false>(ConstArrayPtr{tmpb, n}, cur, lgn);
  return ConstArrayPtr{cur, n};
}
void conquer(Number* const dest, const unsigned int lgn)
{
  static unsigned int size[maxp + 10];
  const unsigned int n = 1u << lgn;
  size[1] = n;
  for (unsigned int cid = 1; cid < n; ++cid) {
    static Number tmpv[maxp + 10];
    const unsigned int left = cid << 1, right = left | 1;
    const unsigned int actualSize = size[cid] >> 1;
    const unsigned int childSize = t[cid].size +
                                   std::max(prod[left].size, prod[right].size) -
                                   1,
                       lgn = ceilLg[childSize];
    dft<true>(t[cid], tmpv, lgn);
    t[left] = transMultiply(tmpv, prod[right], lgn);
    t[left].size = size[left] = actualSize;
    cur += actualSize;
    t[right] = transMultiply(tmpv, prod[left], lgn);
    t[right].size = size[right] = actualSize;
    cur += actualSize;
  }
  const ConstArrayPtr* p = t + n;
  for (unsigned int i = 0; i < n; ++i, ++p)
    dest[i] = p->ptr[0];
}

void evaluation(const ConstArrayPtr& a, const ConstArrayPtr& pos, Number dest[])
{
  if (pos.size == 1) {
    dest[0] = eval(a, pos.ptr[0]);
    return;
  }
  else if (a.size == 1) {
    std::fill(dest, dest + pos.size, a.ptr[0]);
    return;
  }
  cur = mem;
  const unsigned int len = std::max(a.size, pos.size), lgn = ceilLg[len],
                     n = 1u << lgn;
  Product::product<false>(pos, lgn, prod, cur, [](Number* cur, Number v) {
    cur[0] = Number{1};
    cur[1] = -v;
  });
  {
    static Number tmp[maxp + 10];
    ArrayPtr iv{tmp, std::max(n, prod[1].size)}, pv{cur, 0};
    inverse(prod[1], iv);
    ntt<false, true, false>(iv, a, pv);
    pv.size = n;
    t[1] = pv;
    cur += n;
  }
  conquer(dest, lgn);
}
}  // namespace Evaluation
}  // namespace Polynomial

Number readSeq(const unsigned int n)
{
  Number ret{};
  for (Number* i = a; i < a + n; ++i) {
    std::cin >> *i;
    ret += *i;
  }
  return ret;
}
void getModInv(const Number a[], Number dest[], const unsigned int n)
{
  static Number prd[maxp + 10], iprd[maxp + 10];
  prd[0] = a[0];
  for (unsigned int i = 1; i < n; ++i)
    prd[i] = prd[i - 1] * a[i];
  iprd[n - 1] = quickPow(prd[n - 1], mod - 2);
  for (unsigned int i = n - 1; i; --i) {
    iprd[i - 1] = iprd[i] * a[i];
    dest[i] = iprd[i] * prd[i - 1];
  }
  dest[0] = iprd[0];
}
static void readQuery(const unsigned int m, Number sum)
{
  ans[0] = sum;
  trans[0].isFrac = false;
  trans[0].acc = Number{0};
  trans[0].div = Number{1};
  for (unsigned int i = 1; i <= m; ++i) {
    unsigned int op;
    std::cin >> op;
    if (op == 1) {
      Number x;
      std::cin >> x;
      add(i, x);
    }
    else
      inverse(i);
  }
}
void answerFrac(const unsigned int n, const unsigned int q, const Number pc[],
                const unsigned int qid[])
{
  if (!q) return;
  using namespace Polynomial;
  static Number tmem[maxp + 10], dmem[maxp + 10], tval[maxp + 10],
      dval[maxp + 10];
  init(std::max(n, q));
  ArrayPtr prd{tmem}, dprd{dmem};
  const ConstArrayPtr pos{pc, q};
  Product::product(ConstArrayPtr{a, n}, prd, [](Number* cur, Number b) {
    cur[0] = b;
    cur[1] = Number{1};
  });
  derivative(prd, dprd);
  Evaluation::evaluation(prd, pos, tval);
  Evaluation::evaluation(dprd, pos, dval);
  getModInv(tval, tval, q);
  for (unsigned int i = 0; i < q; ++i)
    ans[qid[i]] =
        trans[qid[i]].a * Number{n} + trans[qid[i]].b * tval[i] * dval[i];
}
void answerQuery(const unsigned int n, const unsigned int m)
{
  static Number pc[maxm + 10];
  static unsigned int qid[maxm + 10];
  unsigned int q = 0;
  for (unsigned int i = 1; i <= m; ++i)
    if (!trans[i].isFrac)
      ans[i] = (ans[0] + Number{n} * trans[i].acc) * inverse(trans[i].div);
    else {
      pc[q] = trans[i].c;
      qid[q++] = i;
    }
  answerFrac(n, q, pc, qid);
}

int main()
{
  std::ios::sync_with_stdio(false);
  unsigned int n, m;
  std::cin >> n >> m;
  readQuery(m, readSeq(n));
  answerQuery(n, m);
  std::copy(ans + 1, ans + 1 + m,
            std::ostream_iterator<Number>(std::cout, "\n"));
  return 0;
}