#include <fstream>
#include <random>
using namespace std;
const int maxn = 100;

mt19937_64 rnd;

int main(int argc, char* argv[])
{
    rnd.seed(random_device()());
    ofstream out(argv[1]);
    uniform_int_distribution<unsigned int> dis(1, maxn);
    out << dis(rnd) << endl;
    out.close();
    return 0;
}