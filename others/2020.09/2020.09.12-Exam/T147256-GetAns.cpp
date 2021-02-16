#include <algorithm>
#include <climits>
#include <cstring>
#include <fstream>
#include <functional>
using namespace std;
const unsigned int maxn = 25;
unsigned long long mod = 1e9 + 9;

unsigned int a[maxn + 10], x;
unsigned long long ans;
unsigned int n;
bool sel[maxn + 10];

template <class Operate>
void dfs(const unsigned int pos, const unsigned int acc)
{
  static Operate op;
  if (acc == x) ans = (ans + 1) % mod;
  if (pos == n) return;
  for (unsigned int i = 0; i < n; ++i)
    if (!sel[i]) {
      sel[i] = true;
      dfs<Operate>(pos + 1, op(acc, a[i]));
      sel[i] = false;
    }
}
unsigned int readVal(istream& is, const unsigned int m)
{
  unsigned int ret = 0;
  for (int j = m - 1; j >= 0; --j) {
    unsigned int v;
    is >> v;
    ret |= v << j;
  }
  return ret;
}
int main(int, char* argv[])
{
  ifstream in(argv[1]);
  ofstream fans(argv[2]);
  char op;
  unsigned int m;
  in >> op >> n >> m;
  for (unsigned int* i = a; i < a + n; ++i)
    *i = readVal(in, m);
  x = readVal(in, m);
  switch (op) {
    case '&':
      dfs<bit_and<unsigned int>>(0, UINT_MAX);
      break;
    case '|':
      dfs<bit_or<unsigned int>>(0, 0);
      break;
    case '^':
      dfs<bit_xor<unsigned int>>(0, 0);
      break;
  }
  if (!x && op != '&') --ans;
  fans << ans << "\n";
  return 0;
}