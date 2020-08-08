#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxa = 1e6, maxn = 5000, maxq = 1e5;

int maybe(const char* str, const int def)
{
    return isdigit(str[0]) ? atoi(str) : def;
}
void createSeq(ofstream& in, const unsigned int n, const int mna, const int mxa)
{
    uniform_int_distribution<int> disa(mna, mxa);
    generate_n(ostream_iterator<int>(in, " "), n, [&disa]() { return disa(default_rnd); });
    in << endl;
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
    uniform_int_distribution<unsigned int> disp(1, n);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int p1 = disp(default_rnd), p2 = disp(default_rnd);
        in << min(p1, p2) << " " << max(p1, p2) << endl;
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    ofstream in(argv[5]);
    in << n << " " << q << endl;
    createSeq(in, n, maybe(argv[3], -maxa), maybe(argv[4], maxa));
    createQuery(in, n, q);
    return 0;
}