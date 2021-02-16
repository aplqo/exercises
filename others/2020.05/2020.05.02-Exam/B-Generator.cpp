#include <fstream>
#include <random>
#include <string>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 50000;
const char c[] = "abcdefghijklmnopqrstuvwxyz0123456789";

int main(int argc, char* argv[])
{
  Init();
  ofstream out(argv[2]);
  const unsigned int n = MayBe(stoi(argv[1]), GetRandom(2, maxn));
  const unsigned int m = GetRandom(1, 4);
  out << n << " " << m << endl;

  mt19937_64 rnd;
  rnd.seed(random_device()());
  uniform_int_distribution<unsigned int> dis(0, 35);
  for (unsigned int i = 0; i < n; ++i) {
    for (unsigned int j = 0; j < 4; ++j)
      out << c[dis(rnd)];
    out << endl;
  }

  out.close();
  return 0;
}