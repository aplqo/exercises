#ifdef APTEST
#include "debug_tools/program.h"
#endif
#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>
using namespace std;
const unsigned int maxn = 1e6;
typedef vector<unsigned int> Replace;

unsigned long long pos[maxn + 10];

Replace operator*(const Replace& a, const Replace& b)
{
    Replace ret(a.size());
    for (unsigned int i = 1; i < a.size(); ++i)
        ret[i] = a[b[i]];
    return ret;
}
Replace quickPow(Replace a, unsigned long long e)
{
    Replace ret(a.size());
    iota(ret.begin(), ret.end(), 0);
    for (; e; e >>= 1)
    {
        if (e & 0x01)
            ret = ret * a;
        a = a * a;
    }
    return ret;
}
Replace kth(const unsigned int n, const unsigned int k)
{
    const unsigned long long *pl = pos + 1, *pr = pos + k + 1;
    Replace ret(n + 1);
    for (unsigned int i = 1; i <= n; ++i)
    {
        while (pr + 1 < pos + n + 1 && *(pr + 1) - pos[i] < pos[i] - *pl)
        {
            ++pr;
            ++pl;
        }
        if (pos[i] - *pl >= *pr - pos[i])
            ret[i] = pl - pos;
        else
            ret[i] = pr - pos;
    }
    return ret;
}

int main()
{
#ifndef APTEST
    ios_base::sync_with_stdio(false);
#endif
    unsigned int n, k;
    unsigned long long m;
    cin >> n >> k >> m;
    copy_n(istream_iterator<unsigned long long>(cin), n, pos + 1);
    const Replace& ret = quickPow(kth(n, k), m);
    copy(ret.cbegin() + 1, ret.cend(), ostream_iterator<unsigned int>(cout, " "));
    cout.put('\n');
    return 0;
}