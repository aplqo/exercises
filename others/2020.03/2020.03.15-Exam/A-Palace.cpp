#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
using namespace std;
using num_t = unsigned long long;
constexpr num_t mod = 1e8 + 7; // problem

num_t fact(const unsigned int n)
{
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
    cin >> n;

    num_t d1 = 0, d2 = 1;
    for (unsigned int i = 3; i <= n; ++i)
    {
        const num_t tmp = ((i - 1) * ((d1 + d2) % mod)) % mod;
        d1 = d2;
        d2 = tmp;
    }

    switch (n)
    {
    case 1:
        cout << "orzlxx" << endl;
        break;
    case 2:
        cout << d2 * fact(2) << endl;
        break;
    default:
        cout << (d2 * fact(n)) % mod << endl;
        break;
    }
    return 0;
}