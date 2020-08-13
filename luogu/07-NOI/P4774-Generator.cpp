#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5, maxv = 1e6, maxt = 5;
const long long maxp = 1e12, maxa = 1e12;

template <class T>
void createSeq(ostream& in, const unsigned int n, const T mnv, const T mxv)
{
    uniform_int_distribution<T> dis(mnv, mxv);
    generate_n(ostream_iterator<T>(in, " "), n, bind(dis, ref(default_rnd)));
    in << endl;
}

int main(int, char* argv[])
{
    Init();
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
    const unsigned long long ma = MayBe(atoll(argv[4]), maxa), mp = MayBe(atoll(argv[5]), maxp);
    const unsigned int mv = MayBe(atoi(argv[6]), maxv);
    uniform_int_distribution<unsigned int> disn(1, MayBe(atoi(argv[2]), maxn)), dism(1, MayBe(atoi(argv[3]), maxm));
    ofstream in(argv[7]);
    in << t << endl;
    for (unsigned int i = 0; i < t; ++i)
    {
        const unsigned int n = disn(default_rnd), m = dism(default_rnd);
        in << n << " " << m << endl;
        createSeq(in, n, 0ull, ma);
        createSeq(in, n, 1ull, mp);
        createSeq(in, n, 1u, mv);
        createSeq(in, m, 1u, mv);
    }
    return 0;
}