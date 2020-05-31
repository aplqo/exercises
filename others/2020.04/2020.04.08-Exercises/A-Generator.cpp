#include <fstream>
#include <random>
#include <string>
using namespace std;
const int maxn = 2000;
const long long maxl = 1000000;

mt19937_64 rnd;

template <class T>
T GetRandom(const T a, const T b)
{
    uniform_int_distribution<T> dis(a, b);
    return dis(rnd);
}
template <class T>
T Parse(const char* arg, const T def)
{
    T val = stoll(arg);
    return val ? val : def;
}

int main(int argc, char* argv[])
{
    rnd.seed(random_device()());
    unsigned int n = Parse<unsigned int>(argv[1], maxn);
    long long ma = Parse<long long>(argv[2], maxl);
    ofstream out(argv[3]);
    n = GetRandom<unsigned int>(1, n);

    out << n << endl;
    uniform_int_distribution<unsigned int> da(0, n);
    uniform_int_distribution<long long> db(-ma, ma);
    for (unsigned int i = 0; i < n; ++i)
    {
        out << da(rnd) << " ";
        out << db(rnd) << endl;
    }

    out.close();
    return 0;
}