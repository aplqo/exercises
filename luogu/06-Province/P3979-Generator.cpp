#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <functional>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 100000, maxm = 100000, maxv = 1 << 31;

void createVal(ofstream& in, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(1, mv);
    generate_n(ostream_iterator<unsigned int>(in, " "), n, bind(dis, default_rnd));
    in << "\n";
}
void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << GetRandom(1u, i - 1) << " " << i << "\n";
}
void createQuery(ofstream& in, const unsigned int q, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> disp(1, n), disv(1, mv), diso(1, 3);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int op = diso(default_rnd);
        in << op << " ";
        if (op == 2)
            in << disp(default_rnd) << " " << disp(default_rnd) << " " << disv(default_rnd) << "\n";
        else
            in << disp(default_rnd) << "\n";
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    const unsigned int mv = MayBe(atoi(argv[3]), maxv);
    ofstream in(argv[4]);
    in << n << " " << q << "\n";
    createTree(in, n);
    createVal(in, n, mv);
    in << GetRandom(1u, n) << "\n";
    createQuery(in, q, n, mv);
    return 0;
}