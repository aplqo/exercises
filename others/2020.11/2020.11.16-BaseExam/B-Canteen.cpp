#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
using namespace std;
const unsigned int maxn = 10000, maxm = 400000, maxw = 128;

unsigned int mon[maxn + 10], w[maxm + 10], msum, sumw[maxm + 10];
unsigned int cnt[maxw + 10], origCnt[maxw + 10], mnw, sw = 0, sm = 0;
unsigned int n, m;

unsigned int findK(const unsigned int k)
{
    unsigned int acc = 0;
    for (unsigned int i = 0; i <= maxw; ++i)
        if (acc + cnt[i] >= k)
            return i;
        else
            acc += cnt[i];
    return 0;
}
bool dfs(const unsigned int pos, const unsigned int bus, const unsigned int rem, const unsigned int k)
{
    if (!bus || sm < sw)
        return false;
    if (!k || rem >= sw)
        return true;
    if (!pos || rem < mnw)
    {
        sm -= rem;
        if (dfs(findK(k), bus - 1, mon[bus - 1], k))
            return true;
        sm += rem;
        return false;
    }
    for (int i = min({ rem / pos, cnt[pos], k }); i >= 0; --i)
    {
        cnt[pos] -= i;
        sw -= pos * i;
        sm -= pos * i;
        if (dfs(pos - 1, bus, rem - i * pos, k - i))
            return true;
        sm += pos * i;
        sw += pos * i;
        cnt[pos] += i;
    }
    return false;
}
bool check(const unsigned int k)
{
    if (sumw[k] > msum || w[k] > mon[n])
        return false;
    copy(origCnt, origCnt + maxw + 1, cnt);
    mnw = w[1];
    sw = sumw[k];
    sm = msum;
    return dfs(maxw, n, mon[n], k);
}
static void init(const unsigned int n, const unsigned int m)
{
    sort(mon + 1, mon + 1 + n);
    sort(w, w + 1 + m);
    msum = accumulate(mon + 1, mon + 1 + n, 0u);
    for (unsigned int i = 1; i <= m; ++i)
    {
        sumw[i] = sumw[i - 1] + w[i];
        ++origCnt[w[i]];
    }
}
unsigned int binarySearch(const unsigned int m)
{
    int l = 0, r = m;
    while (l <= r)
    {
        const int mid = (l + r) / 2;
        if (check(mid))
            l = mid + 1;
        else
            r = mid - 1;
    }
    return r;
}
int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    cin >> n;
    copy_n(istream_iterator<unsigned int>(cin), n, mon + 1);
    cin >> m;
    copy_n(istream_iterator<unsigned int>(cin), m, w + 1);
    init(n, m);
    cout << binarySearch(m) << "\n";
    return 0;
}