#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <iostream>
#include <numeric>
using namespace std;
const unsigned int maxn = 1e6;

int sum[maxn + 10];
unsigned int nxt[maxn + 10];

static void initNext(const unsigned int n)
{
    static unsigned int last[maxn * 2 + 10];
    unsigned int* const offset = last;
    for (unsigned int i = 0; i <= n; ++i)
    {
        unsigned int& ptr = offset[sum[i]];
        if (sum[ptr] == sum[i])
            nxt[ptr] = i;
        ptr = i;
    }
}
unsigned int solve(const unsigned int n)
{
    static unsigned int to[maxn + 10];
    initNext(n);
    iota(to, to + 1 + n, 0);
    unsigned int ret = 0;
    for (int i = n - 1; i >= 0; --i)
    {
        if (sum[i] > sum[i + 1])
            continue;
        else
        {
            if (nxt[i] && sum[to[nxt[i]]] >= sum[to[i + 1]])
                to[i] = to[nxt[i]];
            else
                to[i] = to[i + 1];
        }
        ret = max(ret, to[i] - i);
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n;
    cin >> n;
    {
        static char buf[maxn + 10];
        cin >> (buf + 1);
        for (unsigned int i = 1; i <= n; ++i)
            sum[i] = sum[i - 1] + (buf[i] == 'p' ? 1 : -1);
    }
    cout << solve(n) << "\n";
    return 0;
}