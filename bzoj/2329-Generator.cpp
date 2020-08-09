#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxq = 1e5;

void createSeq(ofstream& in, const unsigned int n)
{
    uniform_int_distribution<unsigned int> dis(0, 1);
    generate_n(ostream_iterator<char>(in), n, [&dis]() { return dis(default_rnd) ? '(' : ')'; });
    in << endl;
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
    static const char* str[] = { "Replace", "Query", "Swap", "Invert" };
    uniform_int_distribution<unsigned int> disop(0, 3), disp(1, n), disv(0, 1);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int op = disop(default_rnd), l = disp(default_rnd), r = disp(default_rnd);
        in << str[op] << " ";
        if (op == 1)
        {
            unsigned int tl = min(l, r), tr = max(l, r);
            if ((tr + 1 - tl) & 0x01)
            {
                if (tl != 1)
                    --tl;
                else if (tr != n)
                    ++tr;
                else
                    --tr;
            }
            in << tl << " " << tr;
        }
        else
            in << min(l, r) << " " << max(l, r);
        if (!op)
            in << " " << (disv(default_rnd) ? '(' : ')');
        in << endl;
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    ofstream in(argv[3]);
    in << n << " " << q << endl;
    createSeq(in, n);
    createQuery(in, n, q);
    return 0;
}