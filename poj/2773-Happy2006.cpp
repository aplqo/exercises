#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const int maxn = 1000000;

unsigned int ans[maxn + 10];

inline unsigned int gcd(unsigned int a, unsigned int b) // a > b
{
    unsigned int r = a % b;
    while (r)
    {
        a = b;
        b = r;
        r = a % b;
    }
    return b;
}

inline void solve(const unsigned int m, const unsigned int k)
{
    unsigned int *cur = ans, n;
    unsigned long long res;
    ans[0] = 1;
    for (unsigned int i = 2; i < m; ++i)
        if (gcd(m, i) == 1)
            *(++cur) = i;
    n = cur - ans + 1;
    res = (static_cast<unsigned long long>(k - 1) / n) * m;
    res += k % n ? ans[k % n - 1] : ans[n - 1];
    cout << res << endl;
}
int main()
{
    ios_base::sync_with_stdio(false);
    while (cin)
    {
        unsigned int m, k;
        cin >> m >> k;
        if (cin.fail())
            return 0;
        if (m != 1)
            solve(m, k);
        else
            cout << k << endl;
    }
    return 0;
}