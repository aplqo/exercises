#include <cstring>
#include <fstream>
#include <random>
using namespace std;
const int maxn = 1e5, maxa = 1e7;

mt19937_64 rnd;

unsigned int GetRandom(const unsigned int a, const unsigned int b)
{
    uniform_int_distribution<unsigned int> dis(a, b);
    return dis(rnd);
}

int main(int argc, char* argv[])
{
    unsigned int n = atoi(argv[1]), m, a = atoi(argv[2]);
    ofstream out(argv[3]);
    rnd.seed(random_device()());
    if (!n)
        n = GetRandom(1, maxn);
    if (!a)
        a = maxa;
    m = GetRandom(1, n);
    out << n << " " << m << endl;
    for (unsigned int i = 0; i < n; ++i)
        out << GetRandom(1, a) << endl;
    out.close();
    return 0;
}