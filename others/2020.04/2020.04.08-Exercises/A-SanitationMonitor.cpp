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

unsigned int init(const unsigned int n)
{
    sort(m + 1, m + 1 + n, [](member a, member b) -> bool {
        return a.a != b.a ? a.a < b.a : a.b > b.b;
    });
    member *b, *e = find_if(m + 1, m + 1 + n, [](member i) -> bool { return i.a; });
    b = find_if(m + 1, e, [](member i) -> bool { return i.b > 0; });
    for (unsigned int i = 0; i < e - b; ++i)
        f[0][i + 1] = f[0][i] + (b + i)->b;
    for (unsigned int i = e - b + 1; i <= n; ++i)
        f[0][i] = f[0][e - b];
    return e - m;
}
long long dp(const unsigned int n, const unsigned int beg)
{
    if (n >= beg)
    {
        for (unsigned int i = 0; i <= n - beg; ++i)
        {
            member& cur = m[beg + i];
            for (unsigned int j = 1; j <= n; ++j)
                f[i + 1][j] = max(f[i][j + cur.a - 1] + cur.b, f[i][j]);
        }
    }
    return f[n - beg + 1][1];
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
    cout << dp(n, init(n)) << endl;
    return 0;
}
