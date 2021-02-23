#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxq = 1e5;
constexpr unsigned long long mod = 1e7 + 19;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned long long ret = l.val + r.val;
    return Number{ret >= mod ? ret - mod : ret};
  }
  inline void operator+=(const Number r)
  {
    val += r.val;
    if (val >= mod) val -= mod;
  }
  inline friend Number operator-(const Number l, const Number r)
  {
    return Number{l.val >= r.val ? l.val - r.val : l.val + mod - r.val};
  }
  inline void operator-=(const Number r)
  {
    val = val >= r.val ? val - r.val : val + mod - r.val;
  }
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{(l.val * r.val) % mod};
  }
  inline void operator*=(const Number r) { val = (val * r.val) % mod; }

  unsigned long long val;
};

Number quickPow(Number a, unsigned long long e)
{
  Number ret{1};
  for (; e; e >>= 1) {
    if (e & 0x01) ret *= a;
    a *= a;
  }
  return ret;
}
inline Number inverse(const Number x) { return quickPow(x, mod - 2); }

class Vector {
 public:
  void clear() { cur = val; }
  const int* begin() const { return val; }
  const int* end() const { return cur; }
  void push_back(const int x) { *(cur++) = x; }

 private:
  int val[maxq + 10], *cur;
};
class Array {
 public:
  void init(const int n)
  {
    this->n.val = n % mod;
    tag.multiply.val = tag.inverse.val = 1;
    dirty.clear();
  }
  void assign(const int pos, const Number val)
  {
    const Number delt = (val - tag.add) * tag.inverse;
    valSum = valSum - this->val[pos] + delt;
    this->val[pos] = delt;
    if (!vis[pos]) {
      vis[pos] = true;
      dirty.push_back(pos);
    }
  }
  void add(const Number val) { tag.add += val; }
  void multiply(const Number val, const Number inv)
  {
    if (!val.val) {
      clear();
      tag.multiply.val = tag.inverse.val = 1;
      tag.add.val = 0;
    }
    else {
      tag.multiply *= val;
      tag.inverse *= inv;
      tag.add *= val;
    }
  }
  void fill(const Number val)
  {
    tag.add = val;
    tag.multiply.val = tag.inverse.val = 1;
    clear();
  }
  Number get(const int pos) const { return val[pos] * tag.multiply + tag.add; }
  Number sum() const { return valSum * tag.multiply + tag.add * n; }

 private:
  void clear()
  {
    valSum.val = 0;
    for (int i : dirty) {
      this->val[i].val = 0;
      vis[i] = false;
    }
    dirty.clear();
  }

  struct {
    Number multiply, add{}, inverse;
  } tag;
  Number n, val[maxq + 10]{}, valSum{};
  bool vis[maxq + 10];
  Vector dirty;
} arr;

Number acc;
enum class Operate { Assign, Add, Multiply, Fill, Get, Sum };
struct Query {
  Operate op;
  unsigned int index;
  Number val, inv;

  void execute() const;
} qry[maxq + 10];

static void initQuery(Query* const* const begin, Query* const* const end)
{
  static int tmp[maxq + 10];
  int* cur = tmp;
  for (const Query* const* i = begin; i < end; ++i)
    *(cur++) = (*i)->index;
  std::sort(tmp, cur);
  cur = std::unique(tmp, cur);
  for (Query* const* i = begin; i < end; ++i)
    (*i)->index = std::lower_bound(tmp, cur, (*i)->index) - tmp;
}
void Query::execute() const
{
  switch (op) {
    case Operate::Assign:
      arr.assign(index, val);
      break;
    case Operate::Add:
      arr.add(val);
      break;
    case Operate::Multiply:
      arr.multiply(val, inv);
      break;
    case Operate::Fill:
      arr.fill(val);
      break;
    case Operate::Get:
      acc += arr.get(index);
      break;
    case Operate::Sum:
      acc += arr.sum();
      break;
  }
}

inline int read()
{
  int x;
  std::cin >> x;
  return x;
}
inline int readVal()
{
  static const int imod = mod;
  int x;
  std::cin >> x;
  x %= imod;
  if (x < 0) x += imod;
  return x;
}
void readQuery(const int q)
{
  static Query* raw[maxq + 10];
  Query** cur = raw;
  for (Query* i = qry; i < qry + q; ++i)
    switch (read()) {
      case 1:
        *i = Query{.op = Operate::Assign, .index = read(), .val{readVal()}};
        *(cur++) = i;
        break;
      case 2:
        *i = Query{.op = Operate::Add, .val{readVal()}};
        break;
      case 3: {
        const Number v{readVal()};
        *i = Query{.op = Operate::Multiply, .val = v, .inv = inverse(v)};
        break;
      }
      case 4:
        *i = Query{.op = Operate::Fill, .val{readVal()}};
        break;
      case 5:
        *i = Query{.op = Operate::Get, .index = read()};
        *(cur++) = i;
        break;
      case 6:
        *i = Query{.op = Operate::Sum};
        break;
    }
  initQuery(raw, cur);
}

int main()
{
  std::ios::sync_with_stdio(false);
  int n, q, t;
  std::cin >> n >> q;
  arr.init(n);
  readQuery(q);
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    int pos = read() % q;
    const int b = read() % q;
    for (int j = q; j; --j) {
      pos += b;
      if (pos >= q) pos -= q;
      qry[pos].execute();
    }
  }
  std::cout << acc.val << "\n";
  return 0;
}