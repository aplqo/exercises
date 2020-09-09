#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6;

unsigned int a[maxn + 1], cnt[maxn + 1][2], nxt[maxn + 1], sum[maxn + 1];
static void init(const unsigned int n)
{
    partial_sum(a, a + n + 1, sum);
    unsigned int lst = n + 1;
    for (unsigned int i = n; i > 0; --i)
    {
        cnt[i][0] = cnt[i + 1][0];
        cnt[i][1] = cnt[i + 1][1];
        ++cnt[i][sum[i] & 0x01];
        nxt[i] = lst;
        if (a[i])
            lst = i;
    }
}
unsigned long long count(const unsigned int n)
{
    unsigned long long ret = 0;
    for (unsigned int i = 1; i <= n; ++i)
    {
        if (a[i])
            ret += cnt[nxt[i]][!(sum[i - 1] & 0x01)];
        else
            ret += cnt[nxt[i] + 1][!(sum[i - 1] & 0x01)];
    }
    return ret;
}
unsigned long long solve()
{
    unsigned int n;
    cin >> n;
    for (unsigned int i = 1; i <= n; ++i)
    {
        char c;
        cin >> c;
        a[i] = (c == '1');
    }
    init(n);
    return count(n);
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int id;
    cin >> id;
    cout << solve() << "\n";
    return 0;
}