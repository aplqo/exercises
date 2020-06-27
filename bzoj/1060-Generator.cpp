#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 500000, maxv = 1000000;

mt19937_64 rnd;

void createTree(ofstream& f, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> disw(0, mv);
    for (unsigned int i = 2; i <= n; ++i)
        f << i << " " << GetRandom(1u, i - 1) << " " << disw(rnd) << endl;
}
int main(int argc, char* argv[])
{
    Init();
    Init(rnd);
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << endl;
    in << GetRandom(1u, n) << endl;
    createTree(in, n, MayBe(atoi(argv[2]), maxv));
    return 0;
}