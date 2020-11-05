#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5, maxw = 1e5;

void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << GetRandom(1u, i - 1) << " ";
    in.put('\n');
}
void creatWeight(ofstream& in, const unsigned int n, const unsigned int w)
{
    uniform_int_distribution<unsigned int> dis(0, w);
    generate_n(ostream_iterator<unsigned int>(in, " "), n, bind(dis, ref(default_rnd)));
    in.put('\n');
}
void createOperate(ofstream& in, const unsigned int n, const unsigned int m, const unsigned int mw)
{
    uniform_int_distribution<unsigned int> disv(1, n), diso(1, 5), disw(0, mw);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int op = diso(default_rnd);
        in << op << " ";
        switch (op)
        {
        case 1:
            in << disv(default_rnd);
            break;
        case 2:
            in << disv(default_rnd) << " " << disv(default_rnd) << " " << disw(default_rnd);
            break;
        case 3:
            in << disv(default_rnd) << " " << disw(default_rnd);
            break;
        case 4:
            in << disv(default_rnd) << " " << disv(default_rnd);
            break;
        case 5:
            in << disv(default_rnd);
        }
        in.put('\n');
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    const unsigned int mw = MayBe(atoi(argv[3]), maxw);
    ofstream in(argv[4]);
    in << n << "\n";
    creatWeight(in, n, mw);
    createTree(in, n);
    in << m << "\n";
    createOperate(in, n, m, mw);
    return 0;
}