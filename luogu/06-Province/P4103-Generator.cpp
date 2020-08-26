#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <set>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1000000, maxq = 50000;

void createTree(ofstream& in, const unsigned int n)
{
    for (unsigned int i = 2; i <= n; ++i)
        in << GetRandom(1u, i - 1) << " " << i << "\n";
}
void createQuery(ofstream& in, const unsigned int n, const unsigned int q)
{
    uniform_int_distribution<unsigned int> disp(1, n);
    unsigned int sumk = 2 * n;
    in << q << "\n";
    for (unsigned int i = 0; i < q; ++i)
    {
        set<unsigned int> seq;
        const unsigned int k = GetRandom(1u, min(n, sumk - (q - i - 1)));
        in << k << "\n";
        for (unsigned int j = 0; j < k; ++j)
        {
            unsigned int v;
            do
            {
                v = disp(default_rnd);
            } while (seq.find(v) != seq.end());
            seq.insert(v);
            in << v << " ";
        }
        sumk -= k;
        in << "\n";
    }
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(2, MayBe(atoi(argv[1]), maxn));
    const unsigned int q = GetRandom(1u, min<unsigned int>(n * 2, MayBe(atoi(argv[2]), maxq)));
    ofstream in(argv[3]);
    in << n << "\n";
    createTree(in, n);
    createQuery(in, n, q);
    return 0;
}