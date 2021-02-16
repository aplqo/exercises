#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxl = 1001;
constexpr unsigned long long mod = 1000000007;

class Number {
 public:
  Number() = default;
  Number(unsigned long long v) : val(v % mod){};
#define decl(x)                                                    \
  inline friend Number operator x(const Number l, const Number r); \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline bool notZero() const { return val; }
  friend inline ostream& operator<<(ostream& os, const Number v);

 private:
  unsigned long long val = 0;
};
#define impl(x)                                            \
  inline Number operator x(const Number l, const Number r) \
  {                                                        \
    return l.val x r.val;                                  \
  }
impl(+);
impl(*);
#undef impl
inline Number operator-(const Number l, const Number r)
{
  return l.val + mod - r.val;
}
inline ostream& operator<<(ostream& os, const Number v) { return os << v.val; }

Number po[maxl + 1] = {1};
unsigned int x[maxl + 1];
Number mem[maxl + 1][10][10][3];

static void init(const unsigned int l)
{
  for (unsigned int i = 1; i <= l; ++i)
    po[i] = po[i - 1] * 10;
}
inline Number sub(const unsigned int l, const unsigned int d[])
{
  Number v = 0;
  for (unsigned int i = l; i > 0; --i)
    v = v * 10 + d[i];
  return v;
}
enum ZeroTyp { All = 0, Last = 1, None = 2 };
inline unsigned int nxt(const unsigned int lst, const bool c)
{
  if (lst == None) return None;
  return c ? Last : All;
}
Number f(const unsigned int l, const unsigned int lst1, const unsigned int lst2,
         const bool suc, const bool up, const ZeroTyp zero)
{
  static const auto success = [](unsigned int i, unsigned int lst1,
                                 unsigned int lst2, ZeroTyp zero) -> bool {
    return zero != All && (i == lst2 || i == lst1 && zero != Last);
  };
  static const auto nxt = [](const ZeroTyp lst, const bool c) -> ZeroTyp {
    if (lst != All) return None;
    return c ? Last : All;
  };
  if (!l) return suc;
  if (suc) return up ? sub(l, x) + 1 : po[l];
  if (!up && mem[l][lst1][lst2][zero].notZero())
    return mem[l][lst1][lst2][zero];
  Number ret = 0;
  const unsigned int mi = up ? x[l] : 10;
  for (unsigned int i = 0; i < mi; ++i)
    ret += f(l - 1, lst2, i, success(i, lst1, lst2, zero), false, nxt(zero, i));
  if (up)
    ret += f(l - 1, lst2, x[l], success(x[l], lst1, lst2, zero), true,
             nxt(zero, x[l]));
  return mem[l][lst1][lst2][zero] = ret;
}
unsigned int toNumbers(const char* c, unsigned int out[])
{
  const size_t ret = strlen(c + 1);
  for (unsigned int i = 1; i <= ret; ++i)
    out[ret + 1 - i] = c[i] - '0';
  return ret;
}
unsigned int decrease(unsigned int v[], const unsigned int l)
{
  unsigned int* p =
      find_if(v + 1, v + 1 + l, [](const unsigned int i) { return i; });
  fill(v + 1, p, 9);
  --(*p);
  return l - (!*p && p == v + 1 + l);
}

int main()
{
  static char l[maxl + 1], r[maxl + 1];
  cin >> (l + 1) >> (r + 1);
  const unsigned int l1 = decrease(x, toNumbers(l, x));
  init(maxl);
  const Number v = f(l1, 10, 10, false, true, All);
  memset(mem, 0, sizeof(mem));
  cout << f(toNumbers(r, x), 10, 10, false, true, All) - v << endl;
  return 0;
}