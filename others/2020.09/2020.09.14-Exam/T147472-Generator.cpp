#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 300, maxm = 1e5, maxc = 1e9, maxt = 300, maxv = 300;

void createStore(ofstream& in, const unsigned int n, const unsigned int mc, const unsigned mv, const unsigned int mt)
{
    uniform_int_distribution<unsigned int> disc(1, mc), disv(1, mv), dist(1, mt);
    for (unsigned int i = 0; i < n; ++i)
        in << disc(default_rnd) << " " << disv(default_rnd) << " " << dist(default_rnd) << "\n";
}
void createQuery(ofstream& in, const unsigned int m, const unsigned int mc, const unsigned int mt)
{
    uniform_int_distribution<unsigned int> dism(1, mc), dist(1, mt);
    for (unsigned int i = 0; i < m; ++i)
        in << dism(default_rnd) << " " << dist(default_rnd) << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    ofstream in(argv[6]);
    in << n << " " << m << "\n";
    const unsigned int mc = MayBe(atoi(argv[3]), maxc), mt = MayBe(atoi(argv[5]), maxt);
    createStore(in, n, mc, MayBe(atoi(argv[4]), maxv), mt);
    createQuery(in, m, mc, mt);
    return 0;
}