#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxl = 1000000, maxt = 5;

void createstr(ofstream& f, const unsigned int l, const char ch[],
               const size_t sch)
{
  uniform_int_distribution<size_t> dis(0, sch - 1);
  generate_n(ostream_iterator<char>(f), l,
             [&dis, ch]() { return ch[dis(default_rnd)]; });
  f << endl;
}
int main(int, char* argv[])
{
  Init();
  const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
  const size_t l = strlen(argv[3]);
  ofstream in(argv[4]);
  in << t << endl;
  uniform_int_distribution<unsigned int> dis(1, MayBe(atoi(argv[2]), maxl));
  for (unsigned int i = 0; i < t; ++i)
    createstr(in, dis(default_rnd), argv[3], l);
  return 0;
}