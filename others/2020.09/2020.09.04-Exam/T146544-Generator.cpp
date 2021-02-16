#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 2000;

void createSeq(ofstream& in, const unsigned int n)
{
  uniform_int_distribution<unsigned int> disp(0, 100);
  generate_n(ostream_iterator<float>(in, " "), n,
             [&disp]() { return disp(default_rnd) / 100.0; });
  in << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[2]);
  in << n << " " << GetRandom(1u, n - 1) << "\n";
  createSeq(in, n);
  return 0;
}