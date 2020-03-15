#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num_t = long long;
constexpr num_t mod = 1e9 + 7;

num_t fact(const unsigned int n)
{
    if (!n)
        return 0;
    num_t ret = 1;
    for (unsigned int i = 1; i <= n; ++i)
        ret = (ret * i) % mod;
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    num_t ans, tmp = 0;
    cin >> n;
    ans = fact(n) % mod;
    for (unsigned int i = 1; i < n; ++i)
        tmp = (tmp + i * fact(n - i)) % mod;
    ans = (ans - tmp + mod) % mod;
    ans = (ans * n) % mod;
    cout << ans << endl;
    return 0;
}