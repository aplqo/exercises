#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <random>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e5, maxq = 1e5, maxsd = 1e5;

void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << i << " " << GetRandom(1u, i - 1) << "\n";
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q, const unsigned int s)
{
    static unsigned int buf[maxq + 10];
    createArray(0u, s - q, q, buf + 1);
    sort(buf, buf + q + 1);
    for (unsigned int i = 1; i <= q; ++i)
    {
        const unsigned int d = buf[i] - buf[i - 1] + 1;
        in << d << " ";
        createArray(1u, n, d, ostream_iterator<unsigned int>(in, " "));
        in.put('\n');
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(3, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1, MayBe(atoi(argv[2]), maxq));
    const unsigned int s = GetRandom(q, max(q, MayBe<unsigned int>(atoi(argv[3]), maxsd)));
    ofstream in(argv[4]);
    in << n << " " << q << "\n";
    createTree(in, n);
    createQuery(in, n, q, s);
    return 0;
}