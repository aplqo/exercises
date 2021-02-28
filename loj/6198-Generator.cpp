#include <cstring>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;

int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(1, atoi(argv[1]));
  const unsigned int maxM = std::min(n, atoi(argv[2]));
  std::ofstream in(argv[4]);
  in << n << "\n";
  createArray(argv[3], std::strlen(argv[3]), n,
              std::ostreambuf_iterator<char>(in));
  in.put('\n');
  createArray(1u, maxM, n, std::ostream_iterator<unsigned int>(in, " "));
  return 0;
}
