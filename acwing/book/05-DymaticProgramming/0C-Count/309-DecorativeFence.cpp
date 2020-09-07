#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <cstring>
#include <iostream>
#include <iterator>
#include <set>
using namespace std;
const unsigned int maxn = 20;

unsigned long long f[maxn + 1][maxn + 1][2];
unsigned int ans[maxn + 1];

static void init(const unsigned int n)
{
    f[1][1][0] = f[1][1][1] = 1;
    f[2][1][0] = f[2][2][1] = 1;
    for (unsigned int i = 3; i <= n; ++i)
        for (unsigned int j = 1; j <= i; ++j)
        {
            for (unsigned int k = j; k < i; ++k)
                f[i][j][0] += f[i - 1][k][1];
            for (unsigned int k = 1; k < j; ++k)
                f[i][j][1] += f[i - 1][k][0];
        }
}
void dp(const unsigned int n, const unsigned long long c)
{
    set<unsigned int> num;
    for (unsigned int i = 0; i < n; ++i)
        num.insert(i + 1);
    unsigned long long cnt = 0;
    bool typ;
    for (unsigned int i = 1; i <= n; ++i)
    {
        if (cnt + f[n][i][0] + f[n][i][1] >= c)
        {
            ans[0] = i;
            if (cnt + f[n][i][1] >= c)
                typ = true;
            else
            {
                typ = false;
                cnt += f[n][i][1];
            }
            break;
        }
        cnt += f[n][i][0] + f[n][i][1];
    }
    num.erase(ans[0]);
    for (unsigned int i = 1; i < n; ++i)
    {
        unsigned int pos = 0;
        typ ^= 1;
        for (unsigned int j : num)
        {
            ++pos;
            if ((j > ans[i - 1]) ^ typ)
                continue;
            const unsigned long long rk = f[n - i][pos][typ];
            if (rk + cnt >= c)
            {
                ans[i] = j;
                break;
            }
            cnt += rk;
        }
        num.erase(ans[i]);
    }
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    init(maxn);
    unsigned int k;
    cin >> k;
    for (unsigned int i = 0; i < k; ++i)
    {
        unsigned int n;
        unsigned long long c;
        cin >> n >> c;
        dp(n, c);
        copy(ans, ans + n, ostream_iterator<unsigned int>(cout, " "));
        cout << "\n";
    }
    return 0;
}