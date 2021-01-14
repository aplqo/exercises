#include "debug_tools/generator.h"
#include <cstring>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxt = 1e5, maxe = 1e5, maxz = 1u << 31;

void createQuery(ofstream& in, const unsigned int n, const unsigned int e, const int mnv, const int mxv)
{
    constexpr static char ch[] = "QAPC";
    uniform_int_distribution<unsigned int> disn(1, n), diso(0, 3);
    uniform_int_distribution<int> disv(mnv, mxv);
    for (unsigned int i = e; i; --i)
    {
        const unsigned int op = diso(default_rnd);
        unsigned int l = disn(default_rnd), r = disn(default_rnd);
        if (l > r)
            swap(l, r);
        in << ch[op] << " " << l << " " << r;
        if (op > 1)
            in << " " << disv(default_rnd);
        in << "\n";
    }
}

int main(int, char* argv[])
{
    Init();
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
    const unsigned int e = GetRandom(1, MayBe(atoi(argv[2]), maxe));
    const int mnv = MayBe(atoi(argv[3]), -2147483647), mxv = MayBe(atoi(argv[4]), maxz);
    ofstream in(argv[5]);
    in << t << "\n";
    createArray(mnv, mxv, t, ostream_iterator<int>(in, " "));
    in.put('\n');
    in << e << "\n";
    createQuery(in, t, e, mnv, mxv);
    return 0;
}