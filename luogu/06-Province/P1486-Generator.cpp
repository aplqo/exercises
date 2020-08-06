#include "debug_tools/generator.h"
#include <fstream>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 3e5, maxd = 1e3, maxv = 1e5;

void createOperate(ofstream& in, const unsigned int n, const unsigned int md, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> diso(0, 3), disv(1, mv), disd(1, md), disp(1, n);
    for (unsigned int i = 0; i < n; ++i)
    {
        const unsigned int op = diso(default_rnd);
        switch (op)
        {
        case 0:
            in << "I " << disv(default_rnd) << endl;
            break;
        case 1:
            in << "A " << disd(default_rnd) << endl;
            break;
        case 2:
            in << "S " << disd(default_rnd) << endl;
            break;
        case 3:
            in << "F " << disp(default_rnd) << endl;
            break;
        }
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[4]);
    const unsigned int mp = MayBe(atoi(argv[2]), maxv);
    in << n << " " << GetRandom(1u, mp) << endl;
    createOperate(in, n, mp, MayBe(atoi(argv[3]), maxd));
    return 0;
}
