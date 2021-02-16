#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxl = 1e6, maxt = 1000;

void createStr(ofstream& f, const unsigned int l, const char ch[],
               const size_t siz)
{
  uniform_int_distribution<size_t> dis(0, siz - 1);
  generate_n(ostream_iterator<char>(f), l,
             [&dis, ch]() { return ch[dis(default_rnd)]; });
  f << endl;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
  const size_t sch = strlen(argv[3]);
  ofstream in(argv[4]);
  uniform_int_distribution<unsigned int> dis(1u, MayBe(atoi(argv[2]), maxl));
  for (unsigned int i = 0; i < t; ++i)
    createStr(in, dis(default_rnd), argv[3], sch);
  return 0;
}