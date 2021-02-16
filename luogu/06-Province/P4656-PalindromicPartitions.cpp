#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
using namespace std;
const unsigned int maxn = 1e6;
constexpr unsigned long long base = 131;

#ifdef Use2Module
constexpr unsigned long long mod1 = 1e9 + 7, mod2 = 1e9 + 9;

struct HashValue {
  unsigned long long hash1, hash2;

  inline HashValue operator+(const unsigned long long r) const
  {
    return HashValue{(hash1 + r) % mod1, (hash2 + r) % mod2};
  }
  inline HashValue operator-(const HashValue& r) const
  {
    return HashValue{(hash1 + mod1 - r.hash1) % mod1,
                     (hash2 + mod2 - r.hash2) % mod2};
  }
  inline HashValue operator*(const unsigned int v) const
  {
    return HashValue{(hash1 * v) % mod1, (hash2 * v) % mod2};
  }
  inline HashValue operator*(const HashValue& r) const
  {
    return HashValue{(hash1 * r.hash1) % mod1, (hash2 * r.hash2) % mod2};
  }
  inline bool operator==(const HashValue& r) const
  {
    return hash1 == r.hash1 && hash2 == r.hash2;
  }
};
constexpr HashValue unit{1, 1};
#else
typedef unsigned long long HashValue;
constexpr HashValue unit = 1;
#endif
HashValue po[maxn + 10], has[maxn + 10];
char str[maxn + 10];

static void initPow(const unsigned int n)
{
  po[0] = unit;
  for (unsigned int i = 1; i <= n; ++i)
    po[i] = po[i - 1] * base;
}
void initHash(const char str[], const size_t len)
{
  for (size_t i = 1; i <= len; ++i)
    has[i] = has[i - 1] * base + str[i];
}
inline HashValue substrHash(const unsigned int l, const unsigned int r)
{
  return has[r] - has[l - 1] * po[r + 1 - l];
}
unsigned int greedy(const size_t len)
{
  unsigned int ret = 0;
  size_t lst = 1;
  const size_t ei = len / 2;
  for (size_t i = 1; i <= ei; ++i)
    if (substrHash(lst, i) == substrHash(len + 1 - i, len + 1 - lst)) {
      ret += 2;
      lst = i + 1;
    }
  return ret + (lst != (len + 1) / 2 + 1);
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int t;
  initPow(maxn);
  cin >> t;
  cin.ignore(1000, '\n');
  for (unsigned int i = 0; i < t; ++i) {
    cin.getline(str + 1, maxn + 10);
    const size_t len = strlen(str + 1);
    initHash(str, len);
    cout << greedy(len) << "\n";
  }
  return 0;
}