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
const int maxn = 50, maxm = 50, maxt = 2500;

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[4]);
    in << n << " " << m << " " << GetRandom(1, MayBe(atoi(argv[3]), maxt)) << endl;
    uniform_int_distribution<unsigned int> dis('0', '1');
    for (unsigned int i = 0; i < n; ++i)
    {
        generate_n(ostream_iterator<char>(in), m, [&dis]() -> char { return dis(default_rnd); });
        in << endl;
    }
    return 0;
}