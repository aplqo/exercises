#include <fstream>
#include <random>
#include <string>
using namespace std;
const unsigned int maxn = 2000;

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
    ofstream out(argv[2]);
    unsigned int n = stoi(argv[1]);
    if (!n)
        n = GetRandom(1u, maxn);
    out << n << endl;
    uniform_int_distribution<unsigned int> dis(0, 1);
    for (unsigned int i = 0; i < n; ++i)
    {
        for (unsigned int j = 0; j < n; ++j)
            out << dis(rnd);
        out << endl;
    }
    out.close();
    return 0;
}