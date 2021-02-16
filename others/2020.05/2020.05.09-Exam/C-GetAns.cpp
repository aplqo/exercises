#include <fstream>
#include <map>
#include <numeric>
using namespace std;
const int maxn = 1e5;

unsigned int k;
map<unsigned int, unsigned int> col;
unsigned long long ans;

unsigned int count(const unsigned int x, bool ign)
{
  unsigned int ret = 0;
  for (auto i : col) {
    if (i.first == x && !ign) {
      ign = true;
      if (x == k) ret += i.second - 1;
      continue;
    }
    if (gcd(i.first, x) == k) ret += i.second;
  }
  return ret;
}
void add(const unsigned int x)
{
  ans += count(x, true);
  auto it = col.find(x);
  if (it == col.end()) it = col.insert({x, 0}).first;
  ++(it->second);
}
void remove(const unsigned int x)
{
  auto it = col.find(x);
  if (it == col.end()) return;
  ans -= count(x, false);
  --(it->second);
  if (!it->second) col.erase(it);
}

int main(int argc, char* argv[])
{
  ifstream fin(argv[1]);
  ofstream fans(argv[2]);
  unsigned int n;
  fin >> n >> k;
  for (unsigned int i = 0; i < n; ++i) {
    unsigned int a, b;
    fin >> a >> b;
    if (a)
      add(b);
    else
      remove(b);
    fans << ans << endl;
  }
  return 0;
}