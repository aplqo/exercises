#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 3e5, maxq = 3e5, maxa = 3e5;

void createQuery(std::ofstream& in, const unsigned int m, const unsigned int n)
{
  std::uniform_int_distribution<unsigned int> disp(1, n), disk(2, 5);
  for (unsigned int i = m; i; --i) {
    unsigned int l = disp(default_rnd), r = disp(default_rnd);
    if (l > r) std::swap(l, r);
    in << l << " " << r << " " << disk(default_rnd) << "\n";
  }
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxq));
  const unsigned int ma = std::min<unsigned int>(n, MayBe(atoi(argv[3]), maxa));
  std::ofstream in(argv[4]);
  in << n << " " << m << "\n";
  createArray(1u, ma, n, std::ostream_iterator<unsigned int>(in, " "));
  in.put('\n');
  createQuery(in, m, n);
  return 0;
}