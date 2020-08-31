#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iterator>
using namespace std;
const unsigned int maxn = 2e6;

int a[maxn + 1];

unsigned long long sum(const unsigned int n, const unsigned int beg)
{
    unsigned long long ret = 0;
    int acc = 1;
    for (unsigned int i = beg; i < n; ++i, ++acc)
        ret += abs(a[i] - acc);
    for (unsigned int i = 0; i < beg; ++i, ++acc)
        ret += abs(a[i] - acc);
    return ret;
}
unsigned long long solve(const unsigned int n)
{
    unsigned long long ret = LLONG_MAX;
    for (unsigned int i = 0; i < n; ++i)
        ret = min(ret, sum(n, i));
    return ret;
}

int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n;
    in >> n;
    copy_n(istream_iterator<int>(in), n, a);
    ans << solve(n) << "\n";
    return 0;
}