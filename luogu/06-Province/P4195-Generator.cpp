#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
using namespace std;
using namespace apdebug::generator;
const int maxa = 1e9, maxp = 1e9;
const int maxt = 100;

int main(int, char* argv[])
{
    Init();
    ofstream in(argv[4]);
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
    uniform_int_distribution<unsigned int> disa(2, MayBe(atoi(argv[2]), maxa)), disp(2, MayBe(atoi(argv[3]), maxp));
    for (unsigned int i = 0; i < t; ++i)
        in << disa(default_rnd) << " " << disp(default_rnd) << " " << disa(default_rnd) << endl;
    in << 0 << " " << 0 << " " << 0 << endl;
    return 0;
}