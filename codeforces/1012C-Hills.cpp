#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
#include <iterator>
using namespace std;
const unsigned int maxn = 5000;
constexpr unsigned int inf = UINT_MAX / 2;

unsigned int a[maxn + 10];
unsigned int f[maxn + 10];

inline unsigned int cost(const unsigned int hill, const unsigned int x)
{
    return a[x] >= a[hill] ? a[x] + 1 - a[hill] : 0;
}
inline unsigned int cost(const unsigned int hil1, const unsigned int hil2, const unsigned int x)
{
    const unsigned int mh = min(a[hil1], a[hil2]) - 1;
    return a[x] > mh ? a[x] - mh : 0;
}
void dp(const unsigned int n)
{
    static unsigned int pre[maxn + 10], cst[maxn + 10][maxn + 10];
    for (unsigned int i = 0; i <= n; ++i)
        fill(cst[i] + 1, cst[i] + 1 + n, inf);
    fill(f, f + 1 + n, inf);
    const unsigned int mk = (n + 1) >> 1;
    cst[1][1] = 0;
    f[1] = min(f[1], cost(1, 2));
    for (unsigned int i = 2; i <= n; ++i)
    {
        cst[i][1] = cost(i, i - 1);
        f[1] = min(f[1], cst[i][1] + cost(i, i + 1));
    }
    for (unsigned int k = 2; k <= mk; ++k)
    {
        pre[(k - 1) * 2 - 2] = inf;
        for (unsigned int p = (k - 1) * 2 - 1; p <= n; ++p)
            pre[p] = min(pre[p - 1], cst[p][k - 1] + cost(p, p + 1));
        for (unsigned int j = 2 * k - 1; j <= n; ++j)
        {
            cst[j][k] = cst[j - 2][k - 1] + cost(j - 2, j, j - 1);
            if (j > 3)
                cst[j][k] = min(cst[j][k], pre[j - 3] + cost(j, j - 1));
            f[k] = min(f[k], cst[j][k] + cost(j, j + 1));
        }
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
    dp(n);
    copy(f + 1, f + (n + 1) / 2 + 1, ostream_iterator<unsigned int>(cout, " "));
    cout.put('\n');
    return 0;
}
