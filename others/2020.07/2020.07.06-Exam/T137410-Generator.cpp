#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 5000, maxv = 1e5;

int main(int argc, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    uniform_int_distribution<unsigned int> dis(1, MayBe(atoi(argv[2]), maxv));
    in << n << endl;
    for (unsigned int i = 0; i < n; ++i)
        in << dis(default_rnd) << " " << dis(default_rnd) << endl;
    return 0;
}
