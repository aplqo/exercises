#include "debug_tools/generator.h"
#include <fstream>
#include <random>
#include <string>
using namespace std;
using namespace apdebug::generator;
const int maxd = 1000, maxn = 100;

const unsigned long long pv[] = { 2, 11, 998244353ull };
mt19937_64 rnd;

void createM(ofstream& f, const unsigned int x, const unsigned int y, const unsigned int p)
{
    uniform_int_distribution<unsigned long long> dis(0, p - 1);
    for (unsigned int i = 0; i < x; ++i)
    {
        for (unsigned int j = 0; j < y; ++j)
            f << dis(rnd) << " ";
        f << endl;
    }
}
int main(int argc, char* argv[])
{
    Init();
    rnd.seed(random_device()());
    ofstream in(argv[4]);
    const unsigned int n = GetRandom(1, MayBe(stoi(argv[1]), maxn));
    const unsigned int d = GetRandom(1, MayBe(stoi(argv[2]), maxd));
    const unsigned long long p = pv[MayBe(stoi(argv[3]), GetRandom(0, 2))];
    in << n << " " << d << " " << p << endl;
    createM(in, n, n, p);
    createM(in, d, n, p);
    return 0;
}