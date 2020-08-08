#include "debug_tools/generator.h"
#include <algorithm>
#include <fstream>
#include <iterator>
#include <numeric>
using namespace std;
using namespace apdebug::generator;
const int maxn = 2e5;

void createSeq(ofstream& in, const unsigned int n)
{
    static unsigned int buf[maxn + 10];
    iota(buf, buf + n, 1);
    shuffle(buf, buf + n, default_rnd);
    copy(buf, buf + n, ostream_iterator<unsigned int>(in, " "));
    in << endl;
}
int main(int, char* argv[])
{
    Init();
    const unsigned int n = GetRandom(3, MayBe(atoi(argv[1]), maxn));
    ofstream in(argv[2]);
    in << n << endl;
    createSeq(in, n);
    return 0;
}