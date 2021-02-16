#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e4, maxw = 1e6, maxp = (1u << 31) - 1, maxa = 100;

void createPoint(ofstream& in, const unsigned int n, const unsigned int ma,
                 const unsigned int mx)
{
  uniform_int_distribution<unsigned int> disa(1, ma), disx(1, mx);
  for (unsigned int i = 0; i < n; ++i)
    in << disx(default_rnd) << " " << disx(default_rnd) << " "
       << disa(default_rnd) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  const unsigned int mw = MayBe(atoi(argv[4]), maxw);
  ofstream in(argv[5]);
  in << n << " " << GetRandom(1u, mw) << " " << GetRandom(1u, mw) << "\n";
  createPoint(in, n, MayBe(atoi(argv[2]), maxa), MayBe(atoi(argv[3]), maxp));
  return 0;
}