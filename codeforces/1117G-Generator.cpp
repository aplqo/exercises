#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
#include <utility>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6, maxq = 1e6;

void createP(ofstream& in, const unsigned int n)
{
    static unsigned int buf[maxn + 1];
    iota(buf, buf + n, 1);
    shuffle(buf, buf + n, default_rnd);
    copy(buf, buf + n, ostream_iterator<unsigned int>(in, " "));
    in << "\n";
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
    using pair_t = pair<unsigned int, unsigned int>;
    static pair_t buf[maxq + 1];
    uniform_int_distribution<unsigned int> disp(1, n);
    for (unsigned int i = 0; i < q; ++i)
    {
        const unsigned int a = disp(default_rnd), b = disp(default_rnd);
        buf[i] = make_pair(min(a, b), max(a, b));
    }
    transform(buf, buf + q, ostream_iterator<unsigned int>(in, " "), [](const pair_t& i) { return i.first; });
    in << "\n";
    transform(buf, buf + q, ostream_iterator<unsigned int>(in, " "), [](const pair_t& i) { return i.second; });
    in << "\n";
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(1, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    ofstream in(argv[3]);
    in << n << " " << q << "\n";
    createP(in, n);
    createQuery(in, n, q);
    return 0;
}