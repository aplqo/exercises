#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxd = 1e9, maxc = 1e9, maxl = 1e9;

void createPill(ofstream& in, const unsigned int n, const unsigned int mc)
{
    uniform_int_distribution<unsigned int> dis(0, mc);
    for (unsigned int i = 0; i < n; ++i)
        in << dis(default_rnd) << " " << dis(default_rnd) << "\n";
}
void createWater(ofstream& in, const unsigned int n, const unsigned int mw)
{
    uniform_int_distribution<unsigned int> dis(0, mw);
    generate_n(ostream_iterator<unsigned int>(in, "\n"), n, bind(dis, default_rnd));
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[5]);
    in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxl)) << "\n";
    createPill(in, n, MayBe(atoi(argv[3]), maxd));
    createWater(in, n, MayBe(atoi(argv[4]), maxc));
    return 0;
}