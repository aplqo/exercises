#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 2e6;
constexpr unsigned long long mod = 51123987, inv = 25561994;

class Number {
 public:
  Number() = default;
  Number(const unsigned long long v) : val(v % mod){};
#define decl(x)                                                   \
  friend inline Number operator x(const Number l, const Number r) \
  {                                                               \
    return (l.val + mod) x r.val;                                 \
  }                                                               \
  inline Number& operator x##=(const Number r) { return *this = *this x r; }
  decl(+);
  decl(-);
  decl(*);
#undef decl
  inline void operator++() { *this = *this + 1; }
  inline void operator--() { *this = *this - 1; }
  inline Number div2() const { return *this * inv; }
  friend inline ostream& operator<<(ostream& os, const Number v)
  {
    return os << v.val;
  }

 private:
  unsigned long long val = 0;
};
Number start[maxn + 10], finish[maxn + 10];
unsigned int rad[maxn * 2 + 10];
char str[maxn * 2 + 10];

void read(char str[], const unsigned int n)
{
  for (unsigned int i = 0; i < n; ++i)
    cin >> str[2 * i + 1];
}
void manacher(const unsigned int n)
{
  unsigned int r = 0, c = 0;
  const unsigned int ep = (n - 1) * 2 + 3;
  rad[0] = 1;
  for (unsigned int i = 1; i < ep; ++i) {
    unsigned int j = 0;
    if (i < r && rad[2 * c - i] + i - 1 < r) {
      rad[i] = rad[2 * c - i];
      continue;
    }
    else if (i < r)
      j = r - i;
    for (; i + j < ep && i > j; ++j)
      if (str[i + j] != str[i - j]) break;
    r = i + j - 1;
    rad[i] = j;
    c = i;
  }
}

void GetPos(const unsigned int n)
{
  const unsigned int ed = 2 * (n - 1) + 3;
  for (unsigned int i = 1; i < ed; ++i) {
    if (!rad[i]) continue;
    if (i & 0x01) {
      const unsigned int l = (rad[i] - 1) >> 1, p = (i - 1) >> 1;
      ++start[p - l];
      --start[p + 1];
      ++finish[p];
      --finish[p + l + 1];
    }
    else {
      const unsigned int l = rad[i] >> 1, p = (i >> 1) - 1;
      ++start[p + 1 - l];
      --start[p + 1];
      ++finish[p + 1];
      --finish[p + l + 1];
    }
  }
  partial_sum(start, start + n + 1, start);
  partial_sum(finish, finish + n + 1, finish);
}
Number count(const unsigned int n)
{
  Number acc = 0, ret = 0;
  for (unsigned int i = 0; i < n; ++i) {
    ret += acc * start[i];
    acc += finish[i];
  }
  return ret;
}
Number countPair(const unsigned int n)
{
  const Number sum = accumulate(start, start + n, Number(0));
  return (sum * (sum - 1)).div2();
}

int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  read(str, n);
  manacher(n);
  GetPos(n);
  cout << countPair(n) - count(n) << "\n";
  return 0;
}