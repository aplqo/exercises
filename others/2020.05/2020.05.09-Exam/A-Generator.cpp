#include "debug_tools/generator.h"
#include <fstream>
#include <random>
#include <string>
using namespace std;
using namespace apdebug::generator;
const int maxn = 1e6, maxm = 3e5, maxv = (1ull << 31) - 1;

mt19937_64 rnd;

void createData(const unsigned int n, const unsigned int mv, ofstream& f)
{
    uniform_int_distribution<unsigned int> dis(1, mv);
    for (unsigned int i = 0; i < n; ++i)
        f << dis(rnd) << " ";
    f << endl;
}
void createQuery(const unsigned int n, const unsigned int m, const unsigned int mv, ofstream& f)
{
    uniform_int_distribution<unsigned int> disr(1, n), disv(1, mv);
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int a = disr(rnd), b = disr(rnd), c = disv(rnd);
        while (b < a)
            b = disr(rnd);
        f << a << " " << b << " " << c << endl;
    }
}
int main(int argc, char* argv[])
{
    Init();
    rnd.seed(random_device()());
    ofstream out(argv[4]);
    const unsigned int n = GetRandom(1, MayBe(stoi(argv[1]), maxn));
    const unsigned int m = GetRandom(1, MayBe(stoi(argv[2]), maxm));
    const unsigned int mv = MayBe(stoi(argv[3]), maxv);
    out << n << " " << m << endl;
    createData(n, mv, out);
    createQuery(n, m, mv, out);
    out.close();
    return 0;
}
