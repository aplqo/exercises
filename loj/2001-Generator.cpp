#include <algorithm>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5;

void createTree(std::ofstream& in, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    in << i << " " << GetRandom(1u, i - 1) << "\n";
}
void createQuery(std::ofstream& in, const unsigned int n, const unsigned int m)
{
  std::uniform_int_distribution<unsigned int> disv(1, n), diso(1, 3);
  for (unsigned int i = m; i; --i) {
    const unsigned int op = diso(default_rnd);
    in << op << " " << disv(default_rnd);
    if (op == 2) in << " " << disv(default_rnd);
    in.put('\n');
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  std::ofstream in(argv[3]);
  in << n << " " << m << "\n";
  createTree(in, n);
  createQuery(in, n, m);
  return 0;
}