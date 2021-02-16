#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num_t = unsigned long long;
constexpr num_t mod = 1e9 + 7;
const int maxn = 1e5;

struct edge {
  unsigned int to;
  edge* pre = nullptr;
} ed[maxn * 2 + 10];
edge* head[maxn + 10];
bool typ[maxn + 10];

template <class T>
class Number {
 public:
  Number() = default;
  inline Number(T x) : dat(x % mod) {}
  inline Number operator*(const Number& r) const
  {
    return Number((dat * r.dat) % mod);
  }
  inline Number operator+(const Number& r) const
  {
    return Number((dat + r.dat) % mod);
  }
  inline Number& operator+=(const Number& r)
  {
    return *this = this->operator+(r);
  }
  inline Number& operator*=(const Number& r)
  {
    return *this = this->operator*(r);
  }
  inline operator T() { return dat; }

 private:
  T dat = 0;
};
Number<num_t> f[maxn + 10][2];

inline void AddEdge(const unsigned int from, const unsigned int to)
{
  static edge* cur = ed;
  cur->to = to;
  cur->pre = head[from];
  head[from] = cur;
  ++cur;
}
void dfs(const unsigned int x, const unsigned int fa)
{
  f[x][typ[x]] = 1;
  for (edge* i = head[x]; i; i = i->pre) {
    if (i->to == fa) continue;
    dfs(i->to, x);
    if (typ[x])
      f[x][1] *= f[i->to][0] + f[i->to][1];
    else {
      f[x][1] = f[x][0] * f[i->to][1] + f[x][1] * (f[i->to][0] + f[i->to][1]);
      f[x][0] *= f[i->to][0] + f[i->to][1];
    }
  }
}
int main()
{
#ifndef APTEST
  ios_base::sync_with_stdio(false);
#endif
  unsigned int n;
  cin >> n;
  for (unsigned int i = 1; i <= n; ++i) {
    unsigned int t;
    cin >> t;
    typ[i] = t;
  }
  for (unsigned int i = 1; i < n; ++i) {
    unsigned int a, b;
    cin >> a >> b;
    AddEdge(a, b);
    AddEdge(b, a);
  }
  dfs(1, 0);
  cout << (typ[1] ? f[1][0] + f[1][1] : f[1][1]) << endl;
  return 0;
}