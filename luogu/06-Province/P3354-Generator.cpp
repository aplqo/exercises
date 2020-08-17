#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 100, maxk = 50, maxw = 10000, maxe = 10000;

void createTree(ofstream& in, const unsigned int n, const unsigned int mw, const unsigned int me)
{
    uniform_int_distribution<unsigned int> disw(0, mw), dise(0, me);
    for (unsigned int i = 1; i <= n; ++i)
        in << disw(default_rnd) << " " << GetRandom(0u, i - 1) << " " << dise(default_rnd) << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[5]);
    in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxk)) << endl;
    createTree(in, n, MayBe(atoi(argv[3]), maxw), MayBe(atoi(argv[4]), maxe));
    return 0;
}