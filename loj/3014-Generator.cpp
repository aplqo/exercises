#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 2e5, maxm = maxn;

void createTree(std::ofstream& in, const int n)
{
  for (int i = 2; i <= n; ++i)
    in << i << " " << GetRandom(1, i - 1) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(1, atoi(argv[1])), m = GetRandom(1, atoi(argv[2]));
  std::ofstream in(argv[3]);
  in << n << " " << m << "\n";
  createTree(in, n);
  createArray(1, m, n, std::ostream_iterator<int>(in, " "));
  return 0;
}