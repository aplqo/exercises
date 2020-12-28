#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5;
const unsigned int mod = 998244353;

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[2]);
    in << n << "\n"
       << "1 ";
    createArray(0u, mod - 1, n - 1, ostream_iterator<unsigned int>(in, " "));
    in.put('\n');
    return 0;
}