#include <algorithm>
#include <fstream>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 200, maxx = 1e9;

int main(int, char* argv[])
{
  Init();
  ofstream(argv[3]) << GetRandom(1, MayBe(atoi(argv[2]), maxx)) << " "
                    << GetRandom(1, MayBe(atoi(argv[1]), maxn)) << " "
                    << GetRandom(0, 100) << "\n";
  return 0;
}