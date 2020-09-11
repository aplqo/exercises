#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 20;

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[2]);
    unsigned int v = 1 << n;
    for (unsigned int i = 0; i < 2; ++i)
    {
        const unsigned int nv = GetRandom(0u, v);
        in << nv << " ";
        v -= nv;
    }
    in << v << "\n";
    return 0;
}