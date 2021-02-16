#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <utility>
using namespace std;
#define lowbit(x) ((x) & -(x))
using real_t = long double;
using point = pair<real_t, real_t>;
using res_t = uint_fast32_t;
constexpr unsigned int inf = UINT_MAX;
const int maxn = 18;
constexpr real_t eps = numeric_limits<real_t>::epsilon() * 1e5;
constexpr res_t maxs = res_t(1) << maxn;

class line {
 public:
  line(const point a, const point b) noexcept
  {
    const real_t div = a.first * b.first * (a.first - b.first);
    if (div >= 0 && div <= 0) return;
    this->a = (a.second * b.first - b.second * a.first) / div;
    this->b =
        (b.second * a.first * a.first - a.second * b.first * b.first) / div;
    if (!(this->a < 0)) return;
    fail = false;
  }
  bool test(point p) const noexcept(true)
  {
    return !Compare(calc(p.first), p.second);
  }

  bool fail = true;

 private:
  real_t calc(real_t x) const noexcept { return x * x * a + x * b; }
  static int Compare(real_t a, real_t b)
  {
    if (abs(a - b) < eps)
      return 0;
    else if (a - b > eps)
      return 1;
    else
      return -1;
  }

  real_t a, b;
};
point pig[maxn + 10];
res_t cov[maxn + 5][maxn + 5];
unsigned int f[maxs + 5], lg[maxs + 5];

#ifdef APLOG
namespace aplog {
using log_t = pair<unsigned int, unsigned int>;
ofstream logc, logs, binc;
log_t sel[maxs + 5];
res_t lst[maxs + 5];
unsigned int min(res_t i, res_t j, log_t s)
{
  if (f[i] > f[j] + 1) {
    lst[i] = j;
    sel[i] = s;
    return f[j] + 1;
  }
  return f[i];
}
void PrintBinary(ostream& os, unsigned int val, unsigned int b)
{
  for (unsigned int i = 0, t = 1 << (b - 1); i < b; ++i, t >>= 1)
    os << (val & t ? 1 : 0);
  os << " ";
}
struct stream {
  stream(const char* cfile, const char* cbin, const char* sfile)
  {
    logc.open(cfile);
    binc.open(cbin);
    logs.open(sfile);
  }
  ~stream()
  {
    logc.close();
    binc.close();
    logs.close();
  }
};
void PrintLog(unsigned int n)
{
  const unsigned int dw = log10((res_t(1) << n) - 1) + 1;
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int k = 0; k < i + 1; ++k) {
      logc.width(dw);
      logc << " "
           << " ";
      binc.width(n);
      binc << " "
           << " ";
    }
    for (unsigned int j = i + 1; j < n; ++j) {
      logc.width(dw);
      logc << cov[i][j] << " ";
      PrintBinary(binc, cov[i][j], n);
    }
    logc << endl;
    binc << endl;
  }
  logc << endl;

  for (res_t i = (res_t(1) << n) - 1; i; i = lst[i]) {
    logs << "Info f[i] = " << f[i] << " select=(" << sel[i].first << ","
         << sel[i].second << ")" << endl;
  }
  logs << endl;
}
}  // namespace aplog
#define min(i, j, s) (aplog::min(i, j, s))
#define log_init(c) aplog::stream l(c[1], c[2], c[3])
#define log_print(n) aplog::PrintLog(n)
#else
#define log_init(c)
#define log_print(n)
#define min(i, j, s) (min(f[i], f[j] + 1))
#endif

inline void GetLowbit(const unsigned int n)
{
  res_t t = 1;
  for (unsigned int i = 0; i <= n; ++i, t <<= 1)
    lg[t] = i;
}
inline void solve()
{
  unsigned int n, m;
  cin >> n >> m;
  for (point* i = pig; i < pig + n; ++i) {
    i->first = 0;
    i->second = 0;
    cin >> i->first >> i->second;
  }

  for (unsigned int i = 0; i < n; ++i)
    for (unsigned int j = i + 1; j < n; ++j) {
      line l(pig[i], pig[j]);
      cov[i][j] = 0;
      if (l.fail) continue;
      res_t t = 1;
      for (unsigned int k = 0; k < n; ++k, t <<= 1)
        if (l.test(pig[k])) cov[i][j] |= t;
    }

  for (res_t i = 1; i < (res_t(1) << n); ++i) {
    unsigned int s = lg[lowbit(i)];
    for (unsigned int j = s + 1; j < n; ++j)
      if (cov[s][j]) f[i] = min(i, i & ~cov[s][j], make_pair(s, j));
    f[i] = min(i, i - lowbit(i), make_pair(i, n + 1));
  }

  cout << f[(res_t(1) << n) - 1] << endl;
  log_print(n);
  fill(f + 1, f + (1 << n), inf);
}
int main(int argc, char* argv[])
{
  ios_base::sync_with_stdio(false);
  GetLowbit(maxn);
  fill(f + 1, f + maxs, inf);
  log_init(argv);

  unsigned int t;
  cin >> t;
  for (unsigned int i = 0; i < t; ++i)
    solve();

  return 0;
}