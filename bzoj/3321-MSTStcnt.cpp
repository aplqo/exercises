#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
const unsigned long long mod = 1e9 + 7;

unsigned long long quickPow(unsigned long long a, unsigned long long e)
{
    unsigned long long ret = 1;
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = (ret * a) % mod;
        a = (a * a) % mod;
    }
    return ret;
}
int main()
{
    unsigned long long n, k;
    cin >> n >> k;
    // n^{n-2} * (n-1)^{n(k-1)} * k^{nk-2}
    const unsigned long long v1 = quickPow(n, n - 2), v2 = quickPow(n - 1, n * (k - 1)), v3 = quickPow(k, n * k - 2);
    cout << ((v1 * v2) % mod * v3) % mod << endl;
    return 0;
}