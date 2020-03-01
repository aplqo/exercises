#include <cstring>
#include <fstream>
#include <random>
using namespace std;
const int maxn = 1000000;

mt19937_64 rnd;
bool vis[11];

unsigned int GetRandom(const unsigned int a, const unsigned int b)
{
    uniform_int_distribution<unsigned int> dis(a, b);
    return dis(rnd);
}
int main(int argc, char* argv[])
{
    rnd.seed(random_device()());
    unsigned int n = GetRandom(1, maxn), m = GetRandom(1, 10);
    ofstream in(argv[1]);
    in << 1 << endl;
    in << n << " " << m << endl;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int v = GetRandom(0, 9);
        while (vis[v])
            v = GetRandom(0, 9);
        vis[v] = true;
        in << v << " ";
    }
    in << endl;
    in.close();
    return 0;
}
