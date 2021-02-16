#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 3e5, maxp = (1u << 31) - 1, maxh = 40000;

void createBlock(ofstream& in, const unsigned int n, const unsigned int mp,
                 const unsigned int mh)
{
  uniform_int_distribution<unsigned int> disp(1, mp), dish(1, mh);
  for (unsigned int i = 0; i < n; ++i)
    in << disp(default_rnd) << " " << dish(default_rnd) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[4]);
  in << n << "\n";
  createBlock(in, n, MayBe(atoi(argv[2]), maxp), MayBe(atoi(argv[3]), maxh));
  return 0;
}