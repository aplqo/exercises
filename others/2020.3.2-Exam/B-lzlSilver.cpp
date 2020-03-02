#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num_t = unsigned long long;
constexpr num_t mod = 1e9 + 7;

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    num_t ans = 0;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
        ans = (i % 2 ? ans * 2 + 1 : ans * 2) % mod;
    if (!(n % 2))
        ans = (ans + 1) % mod;
    cout << ans << endl;
    return 0;
}
