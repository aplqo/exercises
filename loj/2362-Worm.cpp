#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <queue>
using namespace std;
using num_t = int;
const int maxn = 1e5;

num_t a[maxn + 10];
queue<num_t> orig, lar, sml;

inline num_t GetMax()
{
  static constexpr num_t ninf = numeric_limits<num_t>::min();
  static const auto front = [](const queue<num_t>& q) -> num_t {
    return q.empty() ? ninf : q.front();
  };
  const num_t ret = max({front(orig), front(lar), front(sml)});
  if (ret == front(orig))
    orig.pop();
  else if (ret == front(lar))
    lar.pop();
  else if (ret == front(sml))
    sml.pop();
  return ret;
}

int main()
{
  ios_base::sync_with_stdio(false);
  int n, m, q, u, v, t;
  cin >> n >> m >> q >> u >> v >> t;
  for_each(a, a + n, [](num_t& i) -> void { cin >> i; });
  sort(a, a + n);
  for (num_t* i = a + n - 1; i >= a; --i)
    orig.push(*i);

  double p = double(u) / v;
  for (int i = 1; i <= m; ++i) {
    const num_t v = GetMax() + (i - 1) * q;
    const num_t part = floor(v * p);
    if (i % t == 0) cout << v << " ";
    lar.push(part - i * q);
    sml.push(v - part - i * q);
  }

  cout << endl;
  for (int i = 1; i <= n + m; ++i) {
    const num_t v = GetMax();
    if (i % t == 0) cout << (v + m * q) << " ";
  }
  return 0;
}