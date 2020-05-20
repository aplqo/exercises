#include "debug_tools/generator.h"
#include <fstream>
#include <random>
#include <string>
using namespace std;
using namespace apdebug::generator;
const int maxn = 2000, maxt = 1e4;

mt19937_64 rnd;

int main(int argc, char* argv[])
{
    Init();
    rnd.seed(random_device()());
    ofstream in(argv[3]);
    const unsigned int mn = MayBe(stoi(argv[1]), maxn);
    const unsigned int t = GetRandom(1, MayBe(stoi(argv[2]), maxt));
    in << t << " " << GetRandom(2, 21) << endl;
    uniform_int_distribution<unsigned int> dis(1, mn);
    for (unsigned int i = 0; i < t; ++i)
        in << dis(rnd) << " " << dis(rnd) << endl;
    return 0;
}