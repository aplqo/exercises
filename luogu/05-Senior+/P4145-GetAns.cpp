#include <algorithm>
#include <cmath>
#include <fstream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 100000;

unsigned long long val[maxn + 1];

int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, m;
    in >> n;
    copy_n(istream_iterator<unsigned long long>(in), n, val + 1);
    in >> m;
    for (unsigned int i = 0; i < m; ++i)
    {
        unsigned int k, l, r;
        in >> k >> l >> r;
        if (l > r)
            swap(l, r);
        if (!k)
            transform<unsigned long long*, unsigned long long*>(val + l, val + r + 1, val + l, sqrtl);
        else
            ans << accumulate(val + l, val + r + 1, 0ull) << endl;
    }
    return 0;
}