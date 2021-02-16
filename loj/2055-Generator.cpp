#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5;

void createQuery(std::ofstream& in, const unsigned int m, const unsigned int n)
{
  std::uniform_int_distribution<unsigned int> disp(1, n), diso(0, 1);
  for (unsigned int i = m; i; --i) {
    unsigned int l = disp(default_rnd), r = disp(default_rnd);
    if (l > r) std::swap(l, r);
    in << diso(default_rnd) << " " << l << " " << r << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  std::ofstream in(argv[3]);
  in << n << " " << m << "\n";
  createPermutation(n, 1u, std::ostream_iterator<unsigned int>(in, " "));
  in.put('\n');
  createQuery(in, m, n);
  in << GetRandom(1u, n) << "\n";
  return 0;
}