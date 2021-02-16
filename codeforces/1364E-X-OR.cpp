#include <climits>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <random>
using namespace std;
const unsigned int maxn = 2048, maxl = 11;
constexpr unsigned int msk = (1 << maxl) - 1;

bool vis[maxn + 10];
unsigned int zero[maxl + 10], p[maxn + 10];

unsigned int ask(const unsigned int i, const unsigned int j)
{
  static bool vis[maxn + 10][maxn + 10];
  static unsigned int val[maxn + 10][maxn + 10];
  if (vis[i][j]) return val[i][j];
  vis[i][j] = vis[j][i] = true;
  unsigned int ret;
  cout << "? " << i << " " << j << endl;
  cin >> ret;
  if (cin.fail()) terminate();
  return val[i][j] = val[j][i] = ret;
}
unsigned int length(const unsigned int v)
{
  for (unsigned int i = maxl; i; --i)
    if (v & (1 << i)) return i;
  return 0;
}
void findZeroBit(const unsigned int n, const unsigned int l)
{
  mt19937 rnd((random_device())());
  uniform_int_distribution<unsigned int> dis(1, n);
  unsigned int unf = l + 1;
  while (unf) {
    unsigned int u, v;
    do {
      u = dis(rnd);
      v = dis(rnd);
    } while (u == v);
    const unsigned int val = ask(u, v);
    for (unsigned int i = 0; i <= l; ++i)
      if (!(val & (1 << i)) && !zero[i]) {
        zero[i] = u;
        --unf;
      }
  }
}
unsigned int getVal(const unsigned int p, const unsigned int l)
{
  unsigned int ret = msk;
  for (unsigned int i = 0; i <= l; ++i)
    ret &= zero[i] == p ? msk - (1 << i) : ask(p, zero[i]);
  return ret;
}
unsigned int findZero(const unsigned int n, const unsigned int l)
{
  p[1] = getVal(1, l);
  vis[1] = true;
  unsigned int idx = 1;
  for (unsigned int i = 2; i <= n; ++i) {
    const unsigned int v = ask(idx, i);
    if ((v | p[idx]) == p[idx]) {
      idx = i;
      p[i] = getVal(i, l);
      vis[i] = true;
    }
  }
  return idx;
}
void writeAns(const unsigned int n, const unsigned int idx)
{
  for (unsigned int i = 1; i <= n; ++i)
    if (!vis[i]) p[i] = ask(i, idx);
  cout << "! ";
  copy(p + 1, p + 1 + n, ostream_iterator<unsigned int>(cout, " "));
  cout << endl;
}
int main()
{
  ios_base::sync_with_stdio(false);
  unsigned int n;
  cin >> n;
  const unsigned int l = length(n);
  findZeroBit(n, l);
  writeAns(n, findZero(n, l));
  return 0;
}