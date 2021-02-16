#include <cstring>
#include <fstream>
#include <random>

#include "debug_tools/generator.h"
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 100000;

void createTree(ofstream& f, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    f << GetRandom(1u, i - 1) << " " << i << endl;
}
int main(int argc, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[2]);
  in << n << " " << GetRandom(1, 2) << endl;
  createTree(in, n);
  return 0;
}