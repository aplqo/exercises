#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 100000, maxq = 100000, maxv = (1 << 30) - 1;

void createA(ofstream& f, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(0, mv);
    generate_n(ostream_iterator<unsigned int>(f, " "), n, [&dis]() { return dis(default_rnd); });
    f << endl;
}
void createQuery(ofstream& f, const unsigned int q, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> disp(1, n), disv(0, mv), diso(1, 2);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int op = diso(default_rnd);
        f << op << " " << (op == 1 ? disp(default_rnd) : disv(default_rnd)) << " " << disv(default_rnd) << endl;
    }
}
int main(int argc, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    const unsigned int mv = MayBe(atoi(argv[3]), maxv);
    ofstream in(argv[4]);
    in << n << " " << q << endl;
    createA(in, n, mv);
    createQuery(in, q, n, mv);
    return 0;
}