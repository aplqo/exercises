#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxnGreedy = 100000, maxnDp = 2000, maxd = 1e9, maxc = 5;

void createArray(ofstream& in, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(0, mv);
    generate_n(ostream_iterator<unsigned int>(in, " "), n, bind(dis, ref(default_rnd)));
    in << "\n";
}
void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << i << " " << GetRandom(1u, i - 1) << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int mc = MayBe(atoi(argv[2]), maxc);
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), mc > 1 ? maxnGreedy : maxnDp));
    ofstream in(argv[4]);
    in << n << "\n";
    createArray(in, n, GetRandom(1, MayBe(atoi(argv[3]), maxd)));
    createArray(in, n, mc);
    createTree(in, n);
    return 0;
}