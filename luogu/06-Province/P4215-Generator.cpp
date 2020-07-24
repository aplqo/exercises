#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxm = 1e5, maxq = 1e5, maxa = 1e5;

unsigned int a[maxn + 1];
void createSeq(ofstream& f, const unsigned int n, const unsigned int mv)
{
    uniform_int_distribution<unsigned int> dis(1, mv);
    generate_n(a + 1, n, [&dis]() { return dis(default_rnd); });
    copy_n(a + 1, n, ostream_iterator<unsigned int>(f, " "));
    f << endl;
}
void createRange(ofstream& f, const unsigned int n, const unsigned int m)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    for (unsigned int i = 0; i < m; ++i)
    {
        const unsigned int p1 = dis(default_rnd), p2 = dis(default_rnd);
        f << min(p1, p2) << " " << max(p1, p2) << endl;
    }
}
void createQuery(ofstream& f, const unsigned int n, const unsigned int mq)
{
    uniform_int_distribution<unsigned int> dis(1, n);
    const unsigned int q = min(accumulate(a + 1, a + 1 + n, 0u), mq);
    f << q << endl;
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int v;
        do
        {
            v = dis(default_rnd);
        } while (!a[v]);
        --a[v];
        f << v << endl;
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(atoi(argv[2]), maxm));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[3]), maxq));
    ofstream in(argv[5]);
    in << n << " " << m << endl;
    createSeq(in, n, MayBe(atoi(argv[4]), maxa));
    createRange(in, n, m);
    createQuery(in, n, q);
    return 0;
}