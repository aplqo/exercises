#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6, maxq = 1e6, maxa = 1e9;

void createQuery(ofstream& in, const unsigned int n, const unsigned int q, const unsigned int ma)
{
    uniform_int_distribution<unsigned int> disp(1, n), disv(1, ma), diso(1, 2);
    for (unsigned int i = q; i; --i)
    {
        const unsigned int op = diso(default_rnd);
        in << op << " " << disp(default_rnd);
        if (op == 1)
            in << " " << disv(default_rnd);
        in.put('\n');
    }
}

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    const unsigned int ma = MayBe(atoi(argv[3]), maxa);
    ofstream in(argv[4]);
    in << n << " " << q << "\n";
    createArray(1u, ma, n, ostream_iterator<unsigned int>(in, " "));
    in.put('\n');
    createQuery(in, n, q, ma);
    return 0;
}