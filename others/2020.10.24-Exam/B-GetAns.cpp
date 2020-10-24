#include <algorithm>
#include <fstream>
#include <functional>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6;

unsigned int a[maxn + 10];

unsigned long long count(const unsigned int n, const unsigned int k)
{
    unsigned long long ret = 0;
    for (unsigned int l = 1; l <= n; ++l)
        for (unsigned int r = l; r <= n; ++r)
            if ((a[r] ^ a[l - 1]) >= k)
                ++ret;
    return ret;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, k;
    in >> n >> k;
    copy_n(istream_iterator<unsigned int>(in), n, a + 1);
    partial_sum(a + 1, a + 1 + n, a + 1, bit_xor<unsigned int>());
    ans << count(n, k) << "\n";
    return 0;
}