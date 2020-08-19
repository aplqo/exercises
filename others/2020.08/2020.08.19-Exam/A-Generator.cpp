#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
using namespace std;
using namespace apdebug::generator;
const int maxn = 5e5, maxd = 1e9, maxk = 1e9;

void createTree(ofstream& in, const unsigned int n, const unsigned int w)
{
    uniform_int_distribution<unsigned int> disw(0, w), disb(0, 1);
    for (unsigned int i = 2; i <= n; ++i)
        in << GetRandom(1u, i - 1) << " " << disw(default_rnd) << " " << disb(default_rnd) << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[4]);
    in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxk)) << endl;
    createTree(in, n, MayBe(atoi(argv[3]), maxd));
    return 0;
}