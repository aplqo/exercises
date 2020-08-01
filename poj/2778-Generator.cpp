#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxm = 10, maxl = 10, maxn = 2000000000;
const char str[] = "ATGC";

int main(int, char* argv[])
{
    Init();
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    const unsigned int l = GetRandom(1, MayBe(atoi(argv[3]), maxl));
    ofstream in(argv[4]);
    in << m << " " << GetRandom(1, MayBe(atoi(argv[1]), maxn)) << endl;
    uniform_int_distribution<unsigned int> dis(0, 3);
    for (unsigned int i = 0; i < m; ++i)
    {
        generate_n(ostream_iterator<char>(in), l, [&dis]() { return str[dis(default_rnd)]; });
        in << endl;
    }
    return 0;
}