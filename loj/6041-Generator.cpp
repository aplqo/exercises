#include <algorithm>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

void createQuery(std::ofstream &in, const int n, const int m)
{
  std::uniform_int_distribution<int> disp(1, n);
  for (int i = 0; i < m; ++i) {
    int p1 = disp(default_rnd), p2 = disp(default_rnd);
    if (p1 > p2) std::swap(p1, p2);
    in << p1 << " " << p2 << "\n";
  }
}

int main(int, char *argv[])
{
  Init();
  const int n = GetRandom(1, atoi(argv[1])), m = GetRandom(1, atoi(argv[2]));
  std::ofstream in(argv[3]);
  in << n << " " << m << "\n";
  createArray<char>("01", 2, n, std::ostreambuf_iterator<char>(in));
  in.put('\n');
  createQuery(in, n, m);
  return 0;
}