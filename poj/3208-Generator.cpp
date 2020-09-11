#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e7, maxt = 1000;

int main(int, char* argv[])
{
    Init();
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
    ofstream in(argv[3]);
    uniform_int_distribution<unsigned int> dis(1, MayBe(atoi(argv[2]), maxn));
    in << t << "\n";
    for (unsigned int i = 0; i < t; ++i)
        in << dis(default_rnd) << "\n";
    return 0;
}