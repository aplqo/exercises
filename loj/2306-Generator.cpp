#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace apdebug::generator;
const int maxn = 1e5, maxNq = 1e5, maxq = 1e5, maxPrice = 1e9, maxExpire = 1e9;

void createVegetable(std::ofstream& in, const unsigned int n,
                     const unsigned int price, const unsigned int expire)
{
  std::uniform_int_distribution<unsigned int> disp(1, price), diss(0, price),
      disx(0, expire), disc(1, expire);
  for (unsigned int i = n; i; --i)
    in << disp(default_rnd) << " " << diss(default_rnd) << " "
       << disc(default_rnd) << " " << disx(default_rnd) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxNq));
  std::ofstream in(argv[6]);
  in << n << " " << GetRandom(1u, n) << " " << q << "\n";
  createVegetable(in, n, MayBe(atoi(argv[3]), maxPrice),
                  MayBe(atoi(argv[4]), maxExpire));
  createArray(0, MayBe(atoi(argv[5]), maxq), q,
              std::ostream_iterator<unsigned int>(in, "\n"));
  return 0;
}