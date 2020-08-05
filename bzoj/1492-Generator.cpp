#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 100000, maxp = 10, maxr = 100, maxs = 1000;

template <class G>
void createStat(ofstream& in, const unsigned int n, const unsigned int mp, const unsigned int mr)
{
    G disp(1, mp), disr(1, mr);
    for (unsigned int i = 0; i < n; ++i)
        in << disp(default_rnd) << " " << disp(default_rnd) << " " << disr(default_rnd) << endl;
}

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[2]), maxn));
    ofstream in(argv[6]);
    in << n << " " << GetRandom(1, MayBe(atoi(argv[2]), maxs)) << endl;
    const unsigned int mp = MayBe(atoi(argv[4]), maxp), mr = MayBe(atoi(argv[5]), maxr);
    if (argv[1][0] == 'F')
        createStat<uniform_real_distribution<long double>>(in, n, mp, mr);
    else
        createStat<uniform_int_distribution<unsigned int>>(in, n, mp, mr);
    return 0;
}