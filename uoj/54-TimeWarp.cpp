#ifdef APTEST
#include "debug_tools/judge.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
const int maxn = 11, maxc = 20, maxm = 100000;
const unsigned int mod = 10007, inv2 = 5004;

struct Number {
  inline friend Number operator+(const Number l, const Number r)
  {
    const unsigned int val = l.val + r.val;
    return Number{val >= mod ? val - mod : val};
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
  inline friend Number operator*(const Number l, const Number r)
  {
    return Number{(l.val * r.val) % mod};
  }
  inline void operator*=(const Number r) { val = (val * r.val) % mod; }

  unsigned int val;
};
static Number mu[maxm + 1], C[maxm + 10][maxc + 1];
static Number power[maxc + 1][maxm + 1];
Number coeff[maxc + 1][maxm + 1][maxn + 1];
unsigned int m[maxn + 1];

static void initPower(const int n, const unsigned int m)
{
  for (unsigned int i = 1; i <= m; ++i) {
    power[1][i].val = i % mod;
    power[0][i].val = 1;
  }
  for (int j = 2; j <= n; ++j) {
    power[j][1].val = 1;
    for (unsigned int i = 2; i <= m; ++i)
      power[j][i] = power[j - 1][i] * power[1][i];
  }
}
static void eular(const int n)
{
  static int mindiv[maxm + 10], primes[maxm + 10], *pcur = primes;
  mu[1].val = 1;
  for (long long i = 2; i <= n; ++i) {
    if (!mindiv[i]) {
      mindiv[i] = i;
      *(pcur++) = i;
      mu[i].val = mod - 1;
    }
    {
      const Number nxtMu{mu[i].val ? mod - mu[i].val : 0};
      for (const int *j = primes; j < pcur && *j * i <= n && *j < mindiv[i];
           ++j) {
        const int v = *j * i;
        mindiv[v] = *j;
        mu[v] = nxtMu;
      }
    }
    const long long v = mindiv[i] * i;
    if (v <= n) mindiv[v] = mindiv[i];
  }
}
static void initC(const int n, const int mxc)
{
  C[0][0].val = 1;
  for (int i = 1; i <= n; ++i) {
    C[i][0].val = 1;
    for (int j = 1; j <= i && j <= mxc; ++j)
      C[i][j] = C[i - 1][j] + C[i - 1][j - 1];
  }
}

static void initCoefficient()
{
  eular(maxm);
  initC(maxm, maxc - 2);
  initPower(maxc, maxm);
  for (int c = 2; c <= maxc; ++c)
    for (int j = 1; j <= maxm; ++j) {
      static const auto getC = [](const int n, const int m) {
        return n >= m ? C[n][m] : Number{0};
      };
      for (int r = 1, p = j; p <= maxm; p += j, ++r)
        coeff[c][p][0] += mu[r] * getC(j - 1, c - 2);
      for (int k = maxn; k >= 0; --k)
        coeff[c][j][k] = coeff[c][j - 1][k] + coeff[c][j][0] * power[k][j];
    }
}

void multiply(Number poly[], const Number p0, const Number p1, const int len)
{
  poly[len].val = 0;
  for (int i = len; i; --i)
    poly[i] = poly[i - 1] * p1 + poly[i] * p0;
  poly[0] *= p0;
}
void multiply(const unsigned int v[], Number dest[], const int n)
{
  dest[0].val = 1;
  for (int i = 1; i <= n; ++i)
    multiply(dest, Number{(2 * m[i]) % mod}, Number{mod - (v[i] + 1) % mod}, i);
}
inline Number product(const unsigned int val[], const int n)
{
  Number ret{1};
  for (const unsigned int *i = val + 1; i < val + n + 1; ++i)
    ret *= Number{*i} * Number{inv2};
  return ret;
}

Number solve(const int n, const int c)
{
  static unsigned int next[maxn + 10];
  std::fill(next + 1, next + n + 1, 1u);
  Number ret{0};
  const unsigned int lim = *std::min_element(m + 1, m + 1 + n);
  for (unsigned int i = 1; i <= lim;) {
    static unsigned int val[maxn + 10];
    static Number f[maxn + 10];

    unsigned int p = m[1] + 1;
    for (int j = 1; j <= n; ++j) {
      val[j] = m[j] / i;
      next[j] = m[j] / val[j] + 1;
      p = std::min(p, next[j]);
    }
    multiply(val, f, n);

    {
      Number acc{0};
      for (int j = 0; j <= n; ++j)
        acc += f[j] * (coeff[c][p - 1][j] - coeff[c][i - 1][j]);
      ret += acc * product(val, n);
    }
    i = p;
  }
  return ret;
}

int main()
{
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);
  initCoefficient();
  int t;
  std::cin >> t;
  for (int i = 0; i < t; ++i) {
    int n, c;
    std::cin >> n >> c;
    std::copy_n(std::istream_iterator<unsigned int>(std::cin), n, m + 1);
    std::cout << solve(n, c).val << "\n";
  }
  return 0;
}