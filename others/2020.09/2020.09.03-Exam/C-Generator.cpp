#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6;

void createStr(ofstream& in, const unsigned int n, const char ch[],
               const size_t sch)
{
  uniform_int_distribution<size_t> disc(0, sch - 1);
  generate_n(ostream_iterator<char>(in), n,
             [&disc, ch]() { return ch[disc(default_rnd)]; });
  in << "\n";
}
int main(int, char* argv[])
{
  Init();
  ofstream in(argv[3]);
  createStr(in, GetRandom(1, MayBe(atoi(argv[1]), maxn)), argv[2],
            strlen(argv[2]));
  return 0;
}