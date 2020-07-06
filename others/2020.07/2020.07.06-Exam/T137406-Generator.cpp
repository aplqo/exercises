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
const int maxn = 100000, maxv = 100000;

int main(int argc, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[3]);
    in << n << endl;
    uniform_int_distribution<unsigned int> dis(0, MayBe(atoi(argv[2]), maxv));
    const auto gen = [&dis]() { return dis(default_rnd); };
    generate_n(ostream_iterator<unsigned int>(in, " "), n, gen);
    in << endl;
    generate_n(ostream_iterator<unsigned int>(in, " "), n, gen);
    return 0;
}
