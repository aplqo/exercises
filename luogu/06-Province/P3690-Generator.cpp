#include "debug_tools/generator.h"
#include <fstream>
#include <iterator>
#include <functional>
#include <algorithm>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxq = 3e5, maxa = 1e9;

void createSeq(ofstream &in, const unsigned int n, const unsigned int ma)
{
    uniform_int_distribution<unsigned int> dis(1, ma);
    generate_n(ostream_iterator<unsigned int>(in, "\n"), n, bind(dis, ref(default_rnd)));
}
void createQuery(ofstream &in, const unsigned int q, const unsigned int n, const unsigned int ma)
{
    uniform_int_distribution<unsigned int> diso(0, 3), disp(1, n), disv(1, ma);
    for(unsigned int i = 0; i < q; ++i)
    {
        const unsigned int op = diso(default_rnd);
        in << op << " " << disp(default_rnd) << " ";
        if(op < 3)
            in << disp(default_rnd);
        else
            in << disv(default_rnd);
        in.put('\n');
    }
}

int main(int, char * argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    const unsigned int ma = MayBe(atoi(argv[3]), maxa);
    ofstream in(argv[4]);
    in << n << " " << q << "\n";
    createSeq(in, n, ma);
    createQuery(in, q, n, ma);
    return 0;
}
