#include <algorithm>
#include <cmath>
#include <fstream>
using namespace std;
const unsigned int maxn = 1e6;
const unsigned long long mod = 998244353;

bool test(const unsigned int per[], const int n, const int k)
{
    for (int i = 0; i < n; ++i)
        if (abs(static_cast<int>(per[i]) - (i + 1)) == k)
            return false;
    return true;
}
unsigned long long solve(const unsigned int n, const unsigned int k)
{
    static unsigned int p[maxn + 1];
    for (unsigned int i = 0; i < n; ++i)
        p[i] = i + 1;
    unsigned long long ret = 0;
    do
    {
        ret = (ret + test(p, n, k));
    } while (next_permutation(p, p + n));
    return ret;
}
int main(int, char* argv[])
{
    ifstream in(argv[1]);
    ofstream ans(argv[2]);
    unsigned int n, k;
    in >> n >> k;
    ans << solve(n, k) << "\n";
    return 0;
}