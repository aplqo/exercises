#include "debug_tools/generator.h"
#include <fstream>
#include <random>
#include <string>
using namespace std;
using namespace apdebug::generator;
using num = unsigned long long;
const unsigned int maxn = 20;
const num maxa = 1e8, maxd = 1e8;

int main(int argc, char* argv[])
{
    Init();
    ofstream out(argv[3]);
    const unsigned int n = GetRandom(1u, MayBe<unsigned int>(stoi(argv[1]), maxn));
    const num ma = MayBe<num>(stoull(argv[2]), maxa);
    out << n << endl;
    {
        mt19937_64 rnd;
        rnd.seed(random_device()());
        uniform_int_distribution<num> dis(1, maxd);
        for (unsigned int i = 0; i < n; ++i)
            out << dis(rnd) << " ";
        out << endl;
    }
    const num a = GetRandom(num(1), ma);
    const num b = GetRandom(a, ma + 1);
    out << a << " " << b << endl;
    return 0;
}
