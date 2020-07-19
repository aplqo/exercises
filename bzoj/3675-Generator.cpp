#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 100000, maxk = 200, maxa = 1e4;

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << " " << GetRandom(1u, min<unsigned int>(n - 1, maxk)) << endl;
    uniform_int_distribution<unsigned int> dis(0, MayBe(atoi(argv[2]), maxa));
    generate_n(ostream_iterator<unsigned int>(in, " "), n, [&dis]() { return dis(default_rnd); });
    in << endl;
    return 0;
}