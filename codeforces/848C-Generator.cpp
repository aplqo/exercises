#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

void createQuery(std::ofstream &in, const int n, const int q, const int mc)
{
  std::uniform_int_distribution<int> disp(1, n), disv(1, mc), diso(1, 2);
  for (int i = q; i; --i) {
    const int op = diso(default_rnd);
    in << op << " ";
    if (op == 1)
      in << disp(default_rnd) << " " << disv(default_rnd) << "\n";
    else {
      int p1 = disp(default_rnd), p2 = disp(default_rnd);
      if (p1 > p2) std::swap(p1, p2);
      in << p1 << " " << p2 << "\n";
    }
  }
}
int main(int, char *argv[])
{
  Init();
  const int n = GetRandom(1, std::atoi(argv[1]));
  const int q = GetRandom(1, std::atoi(argv[2]));
  const int mc = std::min(n, std::atoi(argv[3]));
  std::ofstream in(argv[4]);
  in << n << " " << q << "\n";
  createArray(1, mc, n, std::ostream_iterator<int>(in, " "));
  in.put('\n');
  createQuery(in, n, q, mc);
  return 0;
}