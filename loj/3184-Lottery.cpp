#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 10000, maxq = 100;

unsigned int nxt[maxn + 10], seq[maxq + 10];
unsigned int a[maxn + 10], ans[maxn + 10][maxq + 10];

static void findDiff(const unsigned int n, const unsigned int l)
{
    for (unsigned int i = 1; i <= n - l; ++i)
    {
        unsigned int acc = 0;
        for (unsigned int j = 1; j <= l; ++j)
            acc += a[j] != a[j + i];
        ++ans[1][nxt[acc]];
        ++ans[1 + i][nxt[acc]];
        for (unsigned int j = 2; j + i + l - 1 <= n; ++j)
        {
            acc -= a[j - 1] != a[j - 1 + i];
            acc += a[j + l - 1] != a[j + i + l - 1];
            ++ans[j][nxt[acc]];
            ++ans[j + i][nxt[acc]];
        }
    }
}
static unsigned int initNxt(const unsigned int n)
{
    unsigned int acc = 0;
    nxt[n] = 1;
    for (unsigned int i = 0; i <= n; ++i)
        if (nxt[i])
            nxt[i] = ++acc;
    for (int i = n - 1; i >= 0; --i)
        if (!nxt[i])
            nxt[i] = nxt[i + 1];
    return acc;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, l, q;
    cin >> n >> l;
    copy_n(istream_iterator<unsigned int>(cin), n, a + 1);
    cin >> q;
    for (unsigned int i = 1; i <= q; ++i)
    {
        unsigned int k;
        cin >> k;
        nxt[k] = 1;
        seq[i] = k;
    }
    const unsigned int cn = initNxt(n);
    findDiff(n, l);
    for (unsigned int i = 1; i <= n; ++i)
        partial_sum(ans[i], ans[i] + 1 + cn, ans[i]);
    for (unsigned int i = 1; i <= q; ++i)
    {
        const unsigned int qid = nxt[seq[i]];
        for (unsigned int j = 1; j <= n + 1 - l; ++j)
            cout << ans[j][qid] << " ";
        cout.put('\n');
    }
    return 0;
}