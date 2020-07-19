#include "debug_tools/generator.h"
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 1e5, maxx = 39989, maxy = 1e9;

int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    uniform_int_distribution<unsigned int> disx(1, MayBe(atoi(argv[2]), maxx)), disy(1, MayBe(atoi(argv[3]), maxy));
    uniform_int_distribution<unsigned int> diso(0, 1);
    ofstream in(argv[4]);
    in << n << endl;
    for (unsigned int i = 0; i < n; ++i)
    {
        unsigned int op = diso(default_rnd);
        in << op << " ";
        if (op == 1)
        {
            unsigned int x1, x2;
            do
            {
                x1 = disx(default_rnd);
                x2 = disx(default_rnd);
            } while (x1 == x2);
            in << x1 << " " << disy(default_rnd) << " " << x2 << " " << disy(default_rnd) << endl;
        }
        else
            in << disx(default_rnd) << endl;
    }
    return 0;
}