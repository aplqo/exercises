#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
using namespace apdebug::generator;
const int maxt = 5, maxn = 1e5, maxm = 1e5, maxv = 1e9;

void createSeq(ofstream& in, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(1, mv);
    generate_n(ostream_iterator<unsigned int>(in, " "), n, [&dis]() { return dis(default_rnd); });
    in << endl;
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int m, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(1, n), dist(0, 1), disv(1, mv);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int t = dist(default_rnd);
        if (t)
            in << "C " << dis(default_rnd) << " " << disv(default_rnd) << endl;
        else
        {
            unsigned int a = dis(default_rnd), b = dis(default_rnd);
            if (a > b)
                swap(a, b);
            in << "Q " << a << " " << b << " " << GetRandom(1u, b + 1 - a) << endl;
        }
    }
}
void createCase(ofstream& in, const unsigned int n, const unsigned int m, const unsigned int mv)
{
    in << n << " " << m << endl;
    createSeq(in, n, mv);
    createQuery(in, n, m, mv);
}
int main(int, char* argv[])
{
    Init();
    const unsigned int t = GetRandom(1, MayBe(atoi(argv[1]), maxt));
    uniform_int_distribution<unsigned int> disn(1, MayBe(atoi(argv[2]), maxn)), dism(1, MayBe(atoi(argv[3]), maxm));
    const unsigned int mv = MayBe(atoi(argv[4]), maxv);
    ofstream in(argv[5]);
    in << t << endl;
    for (unsigned int i = 0; i < t; ++i)
        createCase(in, disn(default_rnd), dism(default_rnd), mv);
    return 0;
}