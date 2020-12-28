#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6, maxm = 1000000, maxt = 100, maxa = 1 << 31;

void createTest(ofstream& in, const unsigned int n, const unsigned int m, const unsigned int ma)
{
    in << n << " " << m << "\n";
    createArray(0u, ma, n, ostream_iterator<unsigned int>(in, " "));
    in.put('\n');
    uniform_int_distribution<unsigned int> disa(0, ma), disp(1, n), diso(0, 2);
    for (unsigned int i = m; i; --i)
    {
        const unsigned int op = diso(default_rnd);
        unsigned int u = disp(default_rnd), v = disp(default_rnd);
        if (u > v)
            swap(u, v);
        in << op << " " << u << " " << v;
        if (!op)
            in << " " << disa(default_rnd);
        in << "\n";
    }
}

int main(int, char* argv[])
{
    Init();
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
    uniform_int_distribution<unsigned int> disn(1, MayBe(atoi(argv[2]), maxn)), dism(1, MayBe(atoi(argv[3]), maxm));
    const unsigned int ma = MayBe(atoi(argv[4]), maxa);
    ofstream in(argv[5]);
    in << t << "\n";
    for (unsigned int i = t; i; --i)
        createTest(in, disn(default_rnd), dism(default_rnd), ma);
    return 0;
}