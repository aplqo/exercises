#include <algorithm>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

void createTree(std::ofstream& in, const int n, const int mw)
{
  std::uniform_int_distribution<int> dis(0, mw);
  for (int i = 2; i <= n; ++i)
    in << i << " " << GetRandom(1, i - 1) << " " << dis(default_rnd) << "\n";
}
void createQuery(std::ofstream& in, const int q, const int n)
{
  std::uniform_int_distribution<int> dis(1, n);
  for (int i = q; i; --i)
    in << dis(default_rnd) << " " << dis(default_rnd) << " " << dis(default_rnd)
       << "\n";
}

int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(1, atoi(argv[1])), q = GetRandom(1, atoi(argv[2]));
  std::ofstream in(argv[4]);
  in << "0\n" << n << " " << q << "\n";
  createTree(in, n, atoi(argv[3]));
  createPermutation(n, 1, std::ostream_iterator<int>(in, " "));
  in << "\n";
  createQuery(in, q, n);
  return 0;
}