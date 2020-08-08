#include <algorithm>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 5000;

int a[maxn + 1];

unsigned long long solve(const unsigned int l, const unsigned int r)
{
    unsigned long long ret = 0;
    for (unsigned int i = l; i <= r; ++i)
        for (unsigned int j = i + 1; j <= r; ++j)
            for (unsigned int k = j + 1; k <= r; ++k)
                ret += a[i] + a[j] + a[k] == 0;
    return ret;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, q;
    in >> n >> q;
    copy_n(istream_iterator<int>(in), n, a + 1);
    for (unsigned int i = 0; i < q; ++i)
    {
        unsigned int a, b;
        in >> a >> b;
        ans << solve(a, b) << endl;
    }
    return 0;
}