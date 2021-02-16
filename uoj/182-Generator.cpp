#include <algorithm>
#include <fstream>
#include <iterator>
#include <unordered_set>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 6e4, maxa = 998244352;
constexpr unsigned int mod = 998244353;

std::unordered_set<unsigned int> forbid;
void createA(std::ofstream& in, const unsigned int n, const unsigned int ma)
{
  std::uniform_int_distribution<unsigned int> dis(1, ma);
  forbid.reserve(n);
  for (unsigned int i = n; i; --i) {
    const unsigned int v = dis(default_rnd);
    in << v << " ";
    forbid.insert(mod - v);
  }
  in.put('\n');
}

void createQuery(std::ofstream& in, const unsigned int m, const unsigned int ma)
{
  std::uniform_int_distribution<unsigned int> diso(1, 2), disv(1, ma);
  unsigned long long acc = 0;
  for (unsigned int i = m; i; --i) {
    const unsigned int op = diso(default_rnd);
    in << op;
    if (op == 1) {
      unsigned int v;
      do {
        v = disv(default_rnd);
      } while (forbid.count((v + acc) % mod));
      acc = (acc + v) % mod;
      in << " " << v;
    }
    in << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  const unsigned int ma = MayBe(atoi(argv[3]), maxa);
  std::ofstream in(argv[4]);
  in << n << " " << m << "\n";
  createA(in, n, ma);
  createQuery(in, m, ma);
  return 0;
}