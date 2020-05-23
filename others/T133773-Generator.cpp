#include "debug_tools/generator.h"
#include <fstream>
using namespace std;
using namespace apdebug::generator;
const int maxn = 3e5;
const unsigned long long maxq = 1e16;

int main(int argc, char* argv[])
{
    Init();
    ofstream in(argv[3]);
    in << GetRandom(1, MayBe(stoi(argv[1]), maxn)) << " ";
    const unsigned long long q = GetRandom(1ull, MayBe(stoull(argv[2]), maxq));
    in << q << " ";
    uniform_int_distribution<unsigned long long> dis(1ull, q);
    for (unsigned int i = 0; i < 3; ++i)
        in << dis(default_rnd) << " ";
    return 0;
}