#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <cmath>
#include <cstring>
#include <iostream>
using namespace std;
constexpr unsigned int maxl = 18, maxs = 9 * maxl + 1;

long long po[maxl + 2] = {1};

template <class T>
using statT = T[maxl + 1][maxs + 1][maxs];
struct State {
  State() = default;
  State(unsigned int len, unsigned int dsum, int mod, int con)
      : len(len), dsum(dsum), mod(mod), con(con)
  {
  }
  template <class T>
  T& operator[](statT<T>& s) const
  {
    return s[len][dsum][con];
  }
  unsigned int len, dsum;
  int mod, con;
};
static void init(const unsigned int n)
{
  for (unsigned int i = 1; i <= n; ++i)
    po[i] = po[i - 1] * 10;
}
static statT<unsigned long long> mem = {};
static statT<bool> vis;
unsigned long long f(const State& s)
{
  if (!s.len) return !s.dsum && !s.con ? 1 : 0;
  if (s.len < 2) return s.dsum < 10 ? s.dsum % s.mod == s.con : 0;
  if (s[vis]) return s[mem];
  unsigned long long ret = 0;
  for (int i = 0; i < 10 && i <= s.dsum; ++i)
    ret += f(State(s.len - 1, s.dsum - i, s.mod,
                   ((s.con - i * po[s.len - 1]) % s.mod + s.mod) % s.mod));
  s[vis] = true;
  return s[mem] = ret;
}

unsigned int toDigits(unsigned long long x, int out[])
{
  unsigned int p = 0;
  do {
    out[++p] = x % 10;
    x /= 10;
  } while (x);
  return p;
}
unsigned long long dp(const unsigned long long x)
{
  unsigned long long ret = 0;
  int digits[maxl + 1] = {};
  const unsigned int l = toDigits(x, digits);
  for (int s = 1; s <= 9 * (l - 1) + digits[l]; ++s) {
    unsigned int acc = 0;
    long long v = 0;
    for (unsigned int i = l; i > 0 && acc <= s; --i) {
      for (unsigned int j = 0; j < digits[i] && acc + j <= s; ++j)
        ret += f(State(i - 1, s - acc - j, s,
                       ((s - j * po[i - 1] - v) % s + s) % s));
      acc += digits[i];
      v = (v + digits[i] * po[i - 1]) % s;
    }
    memset(vis, 0, sizeof(vis));
  }
  return ret;
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned long long l, r;
  cin >> l >> r;
  init(ceil(log10(r + 1)));
  cout << dp(r + 1) - dp(l) << endl;
  return 0;
}