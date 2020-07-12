#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstdio>
#include <iostream>
using namespace std;
using real = long double;
const unsigned int maxn = 15, maxk = 100, maxs = (1 << maxn) - 1;

real f[maxk + 2][maxs + 1];
int a[maxn];
unsigned int s[maxn];

real dp(const unsigned int n, const unsigned int k)
{
    const unsigned int msk = (1 << n) - 1;
    for (unsigned int i = k; i; --i)
        for (unsigned int j = 0; j <= msk; ++j)
        {
            for (unsigned int c = 0; c < n; ++c)
                if ((s[c] & j) == s[c])
                    f[i][j] += max<real>({ f[i + 1][j | (1 << c)] + a[c], f[i + 1][j], 0 });
                else
                    f[i][j] += f[i + 1][j];
            f[i][j] /= n;
        }
    return f[1][0];
}

int main()
{
    unsigned int n, k;
    cin >> k >> n;
    for (unsigned int i = 0; i < n; ++i)
    {
        cin >> a[i];
        while (true)
        {
            unsigned int t;
            cin >> t;
            if (!t)
                break;
            s[i] |= 1 << (t - 1);
        }
    }
    printf("%.6Lf\n", dp(n, k));
    return 0;
}