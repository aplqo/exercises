#include <cstring>
#include <fstream>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6;

int main(int, char* argv[])
{
  Init();
  ofstream(argv[2]) << GetRandom(1, MayBe(atoi(argv[1]), maxn)) << endl;
  return 0;
}