#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 40, maxs = maxn * 40;
typedef double real;

unsigned int l[maxn + 1];
bool tri[2][maxs + 1][maxs + 1];

inline real area(const unsigned int a, const unsigned int b, const unsigned int c)
{
    real p = (a + b + c) / real(2.0);
    return sqrt(p * (p - a) * (p - b) * (p - c)) * 100;
}
inline bool test(const unsigned int a, const unsigned int b, const unsigned int c)
{
    return a + b > c && a + c > b && b + c > a;
}
void dp(const unsigned int n, const unsigned int sum)
{
    tri[0][0][0] = true;
    for (unsigned int i = 1; i <= n; ++i)
    {
        const bool cur = i & 0x01, lst = !cur;
        for (unsigned int j = 0; j <= sum / 2; ++j)
            for (unsigned int k = 0; k <= sum / 2; ++k)
            {
                tri[cur][j + l[i]][k] |= tri[lst][j][k];
                tri[cur][j][k + l[i]] |= tri[lst][j][k];
                tri[cur][j][k] |= tri[lst][j][k];
            }
    }
}
int find(const unsigned int n, const unsigned int sum)
{
    real ret = -1;
    const bool t = n & 0x01;
    for (unsigned int i = 1; i <= sum / 2; ++i)
        for (unsigned int j = 1; j <= sum / 2; ++j)
            if (tri[t][i][j] && i + j < sum && test(i, j, sum - i - j))
                ret = max(ret, area(i, j, sum - i - j));
    return ret;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, l + 1);
    const unsigned int sum = accumulate(l + 1, l + 1 + n, 0u);
    dp(n, sum);
    cout << find(n, sum) << endl;
    return 0;
}