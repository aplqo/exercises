#include <algorithm>
#include <charconv>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 2000, maxm = 2000;

void createMatrix(std::ostream& os, const int n, const int m,
                  const unsigned int msk, const bool zero)
{
  unsigned int dat[16], *pos = dat;
  for (unsigned int i = msk; i; i = (i - 1) & msk)
    *(pos++) = i;
  if (zero) *(pos++) = 0;

  std::uniform_int_distribution<int> disp(0, pos - dat - 1);
  auto gen = [&disp, dat]() { return dat[disp(default_rnd)]; };
  for (int i = 0; i < n; ++i) {
    std::generate_n(std::ostream_iterator<int>(os, " "), m, gen);
    os.put('\n');
  }
}

int main(int, char* argv[])
{
  Init();
  const int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
  unsigned int msk;
  std::from_chars(argv[3], argv[3] + std::strlen(argv[3]), msk, 16);
  std::ofstream in(argv[5]);
  in << n << " " << m << "\n";
  createMatrix(in, n, m, msk, !std::strcmp(argv[4], "zero"));
  return 0;
}