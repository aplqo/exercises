#include <cstring>
#include <fstream>
#include <random>
#include <string>
using namespace std;
const unsigned long long maxn = 3e9;
const unsigned int maxt = 1000;

mt19937_64 rnd;

template <class T>
T GetRandom(const T a, const T b)
{
    uniform_int_distribution<T> dis(a, b);
    return dis(rnd);
}
int main(int argc, char* argv[])
{
    rnd.seed(random_device()());
    ofstream out(argv[3]);
    unsigned int t = stoi(argv[1]);
    unsigned long long n = stoull(argv[2]);
    if (!t)
        t = GetRandom(1u, maxt);
    if (!n)
        n = maxn;
    for (unsigned int i = 0; i < t; ++i)
        out << GetRandom(1ull, n) << endl;
    out.close();
    return 0;
}