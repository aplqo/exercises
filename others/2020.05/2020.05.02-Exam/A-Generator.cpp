#include <climits>
#include <cstring>
#include <fstream>
#include <random>
#include <string>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;

int main(int argc, char* argv[])
{
  ofstream out(argv[3]);
  Init();
  if (!strcmp(argv[1], "primary")) {
    const unsigned long long mv = MayBe(stoull(argv[2]), ULLONG_MAX);
    out << GetRandom(1ull, mv) << endl;
  }
  else if (!strcmp(argv[1], "max")) {
    mt19937_64 rnd;
    rnd.seed(random_device()());
    uniform_int_distribution<unsigned int> dis(0, 9);
    const unsigned int width = GetRandom(1, MayBe(stoi(argv[2]), 100));
    out << GetRandom(1, 9);
    for (unsigned int i = 1; i < width; ++i)
      out << dis(rnd);
    out << endl;
  }
  out.close();
  return 0;
}