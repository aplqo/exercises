#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include <utility>
using namespace std;
using pair_t = pair<unsigned int, unsigned int>;
const int maxn = 1e5, maxd = 100;

pair_t fact[maxn * 2 + 10] = {}, *primes[maxn * 2 + 10], *pcur = fact;
unsigned int mindiv[maxn * 2 + 10];

class Number {
 public:
  Number(unsigned int x)
  {
    memset(dat, 0, sizeof(dat));
    do {
      dat[cur] = x % 10;
      x /= 10;
      ++cur;
    } while (x);
  }
  friend ostream& operator<<(ostream& os, const Number& a);
  Number operator*(const Number& r) const
  {
    Number ret = 0;
    for (unsigned int i = 0; i < r.cur; ++i)
      for (unsigned int j = 0; j < cur; ++j)
        ret.dat[i + j] += r.dat[i] * dat[j];
    ret.fix();
    return ret;
  }
  Number& operator*=(const Number& r)
  {
    *this = *this * r;
    return *this;
  }

 private:
  inline void fix()
  {
    for (digit_t* i = dat; i < dat + cur; ++i) {
      *(i + 1) += *i / 10;
      *i %= 10;
    }
    for (; cur < maxd; ++cur) {
      dat[cur + 1] += dat[cur] / 10;
      dat[cur] %= 10;
    }
    for (cur = maxd - 1; !dat[cur - 1] && cur > 0; --cur)
      ;
  }
  using digit_t = unsigned int;
  digit_t dat[maxd + 10] = {};
  unsigned int cur = 0;
};
ostream& operator<<(ostream& os, const Number& a)
{
  for (const Number::digit_t* i = a.dat + a.cur - 1; i > a.dat - 1; --i)
    os << *i;
  return os;
}

inline void eular(const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      *pcur = make_pair(i, 0);
      mindiv[i] = i;
      primes[i] = pcur;
      ++pcur;
    }
    for (pair_t* j = fact; j->first <= mindiv[i] && j->first * mindiv[i] <= n;
         ++j)
      mindiv[j->first * i] = j->first;
  }
}
template <class T>
inline void calc(unsigned int x)
{
  T op;
  while (x > 1) {
    unsigned int& v = primes[mindiv[x]]->second;
    v = op(v, 1);
    x /= mindiv[x];
  }
}
Number QuickPow(unsigned int x, unsigned int e)
{
  Number ret = 1, tmp = x;
  for (unsigned int i = 1; e; i <<= 1, tmp *= tmp)
    if (e & i) {
      ret *= tmp;
      e ^= i;
    }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  eular(n * 2 + 1);
  for (unsigned int i = n * 2; i > n * 2 - n; --i)
    calc<plus<unsigned int>>(i);
  for (unsigned int i = 1; i <= n + 1; ++i)
    calc<minus<unsigned int>>(i);
  Number ans = 1;
  for (pair_t* i = fact; i < pcur; ++i)
    ans *= QuickPow(i->first, i->second);
  cout << ans << endl;
  return 0;
}