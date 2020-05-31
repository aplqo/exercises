/*
* Luogu team T128880 
*/
#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
using namespace std;
using num_t = long long;
const int maxn = 2000;

struct member
{
    num_t b;
    unsigned int a;
} m[maxn + 10];
num_t f[maxn + 10][maxn + 10];

long long dp(const unsigned int n)
{
    sort(m + 1, m + 1 + n, [](member a, member b) -> bool {
        return a.a != b.a ? a.a < b.a : a.b > b.b;
    }); // required!
    for (unsigned int i = 1; i <= n; ++i)
    {
        const member cur = m[i];
        for (unsigned int j = n; j > 0; --j)
        {
            const unsigned int p = min(j + cur.a - 1, n);
            f[i][j] = max(f[i - 1][j], f[i - 1][p] + cur.b);
        }
    }
    num_t ret = 0;
    for (unsigned int i = 0; i <= n; ++i)
        ret = max(ret, f[i][1]);
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    for (member* i = m + 1; i < m + 1 + n; ++i)
        cin >> i->a >> i->b;
    cout << dp(n) << endl;
    return 0;
}
