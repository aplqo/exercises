#include "debug_tools/generator.h"
#include <algorithm>
#include <cstring>
#include <fstream>
#include <random>
using namespace std;
using apdebug::generator::default_rnd;
using apdebug::generator::GetRandom;
using apdebug::generator::Init;
using apdebug::generator::MayBe;
const int maxn = 100000, maxq = 10000, maxa = 1e9;
const long long maxv = 1e18;

void createSeq(ofstream& f, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(1, mv);
    generate_n(ostream_iterator<unsigned int>(f, " "), n, [&dis]() { return dis(default_rnd); });
    f << endl;
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int q, const unsigned int mv, const unsigned long long ma)
{
    uniform_int_distribution<unsigned int> disp(0, n - 1), disa(1, mv), disop(0, 1);
    uniform_int_distribution<unsigned long long> disx(1, ma);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int o = disop(default_rnd);
        if (o == 0)
            f << "QUERY " << disx(default_rnd) << endl;
        else
            f << "MODIFY " << disp(default_rnd) << " " << disa(default_rnd) << endl;
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    const unsigned int ma = MayBe(atoi(argv[3]), maxa);
    ofstream in(argv[5]);
    in << n << endl;
    createSeq(in, n, ma);
    in << q << endl;
    createQuery(in, n, q, ma, MayBe(atoll(argv[4]), maxv));
    return 0;
}