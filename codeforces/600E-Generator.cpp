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
const int maxn = 1e5, maxc = maxn;

void createColor(ofstream& f, const unsigned int n, const unsigned int mc)
{
    uniform_int_distribution<unsigned int> dis(1, mc);
    generate_n(ostream_iterator<unsigned int>(f, " "), n, [&dis]() { return dis(default_rnd); });
    f << endl;
}
void createTree(ofstream& f, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        f << GetRandom(1u, i - 1) << " " << i << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[2]), maxn));
    ofstream in(argv[3]);
    in << n << endl;
    createColor(in, n, min(n, MayBe<unsigned int>(atoi(argv[2]), maxc)));
    createTree(in, n);
    return 0;
}