#include <algorithm>
#include <climits>
#include <fstream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 5000;
constexpr unsigned int inf = UINT_MAX;

unsigned int x[maxn + 10];

unsigned int minCloumn(const unsigned int n, const unsigned int seq[])
{
    unsigned int acc = 0, ret = 0;
    for (const unsigned int* i = seq; i < seq + n; ++i)
    {
        if (!acc)
        {
            ++ret;
            acc = x[*i];
        }
        else
            acc = min(acc - 1, x[*i]);
    }
    return ret;
}
unsigned int enumerate(const unsigned int n)
{
    static unsigned int seq[maxn + 10];
    unsigned int ret = inf;
    iota(seq, seq + n, 1);
    do
    {
        ret = min(ret, minCloumn(n, seq));
    } while (next_permutation(seq, seq + n));
    return ret;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n;
    in >> n;
    copy_n(istream_iterator<unsigned int>(in), n, x + 1);
    const unsigned int v = enumerate(n);
    ans << v << "\n";
    return 0;
}