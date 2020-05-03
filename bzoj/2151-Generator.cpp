#include "debug_tools/generator.h"
#include <fstream>
#include <random>
#include <string>
using namespace std;
using namespace apdebug::generator;
const int maxn = 200000, maxa = 1000;

int main(int argc, char* argv[])
{
    Init();
    ofstream out(argv[3]);
    unsigned int n = MayBe(stoi(argv[1]), GetRandom(1, maxn));
    int absa = MayBe(stoi(argv[2]), maxa);
    out << n << " " << GetRandom(1u, n) << endl;

    {
        mt19937_64 rnd;
        rnd.seed(random_device()());
        uniform_int_distribution<int> dis(-absa, absa);
        for (unsigned int i = 0; i < n; ++i)
            out << dis(rnd) << " ";
    }
    return 0;
}