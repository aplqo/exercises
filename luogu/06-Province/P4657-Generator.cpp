#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>

#include "debug_tools/generator.h"
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxv = 100, maxp = 1e9;

void createTree(ofstream& in, const unsigned int n)
{
  for (unsigned int i = 2; i <= n; ++i)
    in << i << " " << GetRandom(1u, i - 1) << "\n";
}
int main(int, char* argv[])
{
  Init();
  const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
  ofstream in(argv[4]);
  in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxv)) << "\n";
  createArray(0, MayBe(atoi(argv[3]), maxp), n,
              ostream_iterator<unsigned int>(in, " "));
  in.put('\n');
  createTree(in, n);
  return 0;
}