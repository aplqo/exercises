#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <climits>
#include <iostream>
using namespace std;
const unsigned int maxn = 5e5;
constexpr unsigned int inf = UINT_MAX / 2;

char str[maxn + 10];
unsigned int mx[maxn + 1][3], mn[maxn + 1][3];

unsigned int dp(const unsigned int pos)
{
    if (str[pos] == '0')
    {
        mx[pos][0] = 1;
        mn[pos][0] = 1;
        return 1;
    }
    unsigned int ret;
    fill(mn[pos], mn[pos] + 3, inf);
    if (str[pos] == '1')
    {
        ret = dp(pos + 1);
        for (unsigned int i = 0; i < 3; ++i)
            for (unsigned int j = 0; j < 3; ++j)
                if (i != j)
                {
                    mx[pos][i] = max(mx[pos][i], mx[pos + 1][j]);
                    mn[pos][i] = min(mn[pos][i], mn[pos + 1][j]);
                }
    }
    else
    {
        const unsigned int ch1 = pos + 1;
        ret = dp(pos + 1);
        const unsigned int ch2 = ch1 + ret;
        ret += dp(ch2);
        for (unsigned int i = 0; i < 3; ++i)
        {
            mx[pos][i] = max(mx[ch1][(i + 1) % 3] + mx[ch2][(i + 2) % 3], mx[ch2][(i + 1) % 3] + mx[ch1][(i + 2) % 3]);
            mn[pos][i] = min(mn[ch1][(i + 1) % 3] + mn[ch2][(i + 2) % 3], mn[ch2][(i + 1) % 3] + mn[ch1][(i + 2) % 3]);
        }
    }
    ++mx[pos][0];
    ++mn[pos][0];
    return ret + 1;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> str;
    dp(0);
    cout << *max_element(mx[0], mx[0] + 3) << " " << *min_element(mn[0], mn[0] + 3) << "\n";
    return 0;
}